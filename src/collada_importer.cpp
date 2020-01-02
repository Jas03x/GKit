#include <gk/collada_importer.hpp>

#include <cstdio>
#include <cstring>

#include <algorithm>
#include <array>
#include <vector>

#include <gk/string.hpp>
#include <gk/string_buffer.hpp>
#include <gk/xml.hpp>

struct Param
{
    enum NAME
    {
        X = 1,
        Y = 2,
        Z = 3,
        S = 4,
        T = 5,
        R = 6,
        G = 7,
        B = 8,
        A = 9,
        JOINT = 10,
        TRANSFORM = 11,
        WEIGHT = 12
    };

    enum TYPE
    {
        NAME     = 1,
        FLOAT    = 2,
        FLOAT4X4 = 3
    };

    unsigned char name;
    unsigned char type;
};

struct Accessor
{
    const std::string* source;
    unsigned int count;
    unsigned int stride;
    
    unsigned int num_params;
    std::array<Param, 4> params;
};

struct Technique
{
    Accessor accessor;
};

struct SourceArray
{
    enum 
    {
        FLOAT = 1,
        NAME  = 2
    };

    const std::string* id;
    unsigned int count;
    unsigned int type;

    union
    {
        float* float_array;
        std::string* name_array;
    };
};

struct Source
{
    const std::string* id;
    SourceArray array;
    Technique technique_common;
};

struct Input
{
    enum
    {
        POSITION = 1,
        VERTEX = 2,
        NORMAL = 3,
        TEXCOORD = 4,
        COLOR = 5,
        JOINT = 6,
        INV_BIND_MATRIX = 7,
        WEIGHT = 8
    };

    const std::string* source;
    unsigned int offset;
    unsigned int set;
    unsigned int semantic;
};

struct VertexArray
{
    const std::string* id;
    Input input;
    Source* source;
};

struct TriangleArray
{
    const std::string* material;
    unsigned int count;
    unsigned short* indices;
    
    unsigned int num_inputs;
    std::array<Input, 4> inputs;
};

struct Mesh
{
    std::map<std::string, Source*> sources;
    VertexArray vertices;

    unsigned int num_triangle_arrays;
    TriangleArray* triangle_arrays;
};

struct Joints
{
    std::array<Input, 2> inputs;

    Source* joints;
    Source* bind_poses;
};

struct VertexWeights
{
    std::vector<unsigned short> v_count_array;
    std::vector<unsigned short> v_index_array;
    unsigned int count;

    std::array<Input, 2> inputs;
    Source* joints;
    Source* weights;
};

struct Skin
{
    Mesh* source;
    float bind_shape_matrix[16];
    std::map<std::string, Source*> sources;

    Joints joints;
    VertexWeights vertex_weights;
};

struct Geometry
{
    const std::string* id;
    const std::string* name;

    Mesh mesh;
};

struct Controller
{
    const std::string* id;
    const std::string* name;

    Skin skin;
};

struct Collada
{
    std::map<std::string, Geometry*> geometry_library;
    std::map<std::string, Controller*> controller_library;
};

class ColladaReader
{
private: // data
    bool m_status;
    Collada* m_data;

    StringBuffer m_strbuf;

    std::vector<float> m_float_buffer;
    std::vector<unsigned short> m_ushort_buffer;
    std::vector<std::string> m_string_buffer;

private: // helper functions:
    const XML::Node* find_child(const XML::Node* node, const std::string& name)
    {
        const XML::Node* child = node->find_child(name);
        if(child == nullptr)
        {
            m_status = false;
            printf("error: could not find child \"%s\"\n", name.c_str());
        }
        return child;
    }

    const XML::ChildList* find_children(const XML::Node* node, const std::string& name)
    {
        const XML::ChildList* children = node->find_children(name);
        if(children == nullptr)
        {
            m_status = false;
            printf("error: could not find children \"%s\"\n", name.c_str());
        }
        return children;
    }

    const std::string* find_attribute(const XML::Node* node, const std::string& name)
    {
        const std::string* attribute = node->find_attribute(name);
        if(attribute == nullptr)
        {
            m_status = false;
            printf("error: could not find attribute \"%s\"\n", name.c_str());
        }
        return attribute;
    }

private: // methods
    ColladaReader(Collada* data)
    {
        m_data = data;
        m_status = true;
    }

    void parse_float_array(const std::string& text)
    {
        m_float_buffer.clear();

        char c = 0;
        for(unsigned int i = 0 ;; i++)
        {
            c = text[i];

            if(IS_NUM(c) || (c == '.') || (c == '-') || (c == 'e'))
            {
                m_strbuf.push_back(c);
            }
            else if(m_strbuf.size() > 0)
            {
                m_float_buffer.push_back(m_strbuf.to_float());
            }

            if(c == 0)
            {
                break;
            }
        }
    }

    void parse_ushort_array(const std::string& text)
    {
        m_ushort_buffer.clear();

        char c = 0;
        for(unsigned int i = 0 ;; i++)
        {
            c = text[i];

            if(IS_NUM(c) || (c == '.') || (c == '-') || (c == 'e'))
            {
                m_strbuf.push_back(c);
            }
            else if(m_strbuf.size() > 0)
            {
                m_ushort_buffer.push_back(m_strbuf.to_float());
            }

            if(c == 0)
            {
                break;
            }
        }
    }

    void parse_name_array(const std::string& text)
    {
        m_ushort_buffer.clear();

        char c = 0;
        for(unsigned int i = 0 ;; i++)
        {
            c = text[i];

            if(IS_ALPHA_NUM(c) || (c == '_'))
            {
                m_strbuf.push_back(c);
            }
            else if(m_strbuf.size() > 0)
            {
                m_string_buffer.push_back(std::string(m_strbuf.data(), m_strbuf.size()));
            }

            if(c == 0)
            {
                break;
            }
        }
    }

    void read_input(const XML::Node* node, Input& input)
    {
        const std::string* semantic = nullptr;

        semantic = find_attribute(node, "semantic");
        if(m_status)
        {
            input.source = find_attribute(node, "source");
        }

        if(m_status)
        {
            unsigned int type = 0;

            const char* c_str = semantic->c_str();
            switch(c_str[0])
            {
                case 'C':
                {
                    type = (strcmp(c_str, "COLOR") == 0) ? Input::COLOR : 0;
                    break;
                }
                case 'I':
                {
                    type = (strcmp(c_str, "INV_BIND_MATRIX") == 0) ? Input::INV_BIND_MATRIX : 0;
                    break;
                }
                case 'J':
                {
                    type = (strcmp(c_str, "JOINT") == 0) ? Input::JOINT : 0;
                    break;
                }
                case 'N':
                {
                    type = (strcmp(c_str, "NORMAL") == 0) ? Input::NORMAL : 0;
                    break;
                }
                case 'P':
                {
                    type = (strcmp(c_str, "POSITION") == 0) ? Input::POSITION : 0;
                    break;
                }
                case 'T':
                {
                    type = (strcmp(c_str, "TEXCOORD") == 0) ? Input::TEXCOORD : 0;
                    break;
                }
                case 'V':
                {
                    type = (strcmp(c_str, "VERTEX") == 0) ? Input::VERTEX : 0;
                    break;
                }
                case 'W':
                {
                    type = (strcmp(c_str, "WEIGHT") == 0) ? Input::WEIGHT : 0;
                    break;
                }
                default: { break; }
            }

            if(type != 0)
            {
                input.semantic = type;
            }
            else
            {
                m_status = false;
                printf("invalid semantic: \"%s\"\n", semantic->c_str());
            }
        }

        if(m_status)
        {
            const std::string* offset = node->find_attribute("offset");
            if(offset != nullptr) {
                input.offset = std::stoi(*offset);
            }

            const std::string* set = node->find_attribute("set");
            if(set != nullptr) {
                input.set = std::stoi(*set);
            }
        }
    }

    void read_vertex_array(const XML::Node* node, VertexArray* array)
    {
        array->id = find_attribute(node, "id");

        if(m_status)
        {
            const XML::Node* input = find_child(node, "input");
            if(m_status)
            {
                read_input(input, array->input);
            }
        }
    }

    void read_param(const XML::Node* node, Param& param)
    {
        const std::string* name = nullptr;
        const std::string* type = nullptr;

        name = find_attribute(node, "name");
        if(m_status)
        {
            type = find_attribute(node, "type");
        }

        if(m_status)
        {
            const char* str = name->c_str();
            
            if(name->size() == 1)
            {
                switch(str[0])
                {
                    case 'X': { param.name = Param::NAME::X; break; }
                    case 'Y': { param.name = Param::NAME::Y; break; }
                    case 'Z': { param.name = Param::NAME::Z; break; }
                    case 'S': { param.name = Param::NAME::S; break; }
                    case 'T': { param.name = Param::NAME::T; break; }
                    case 'R': { param.name = Param::NAME::R; break; }
                    case 'G': { param.name = Param::NAME::G; break; }
                    case 'B': { param.name = Param::NAME::B; break; }
                    case 'A': { param.name = Param::NAME::A; break; }
                    default: { break; }
                }
            }
            else
            {
                switch(str[0])
                {
                    case 'J':
                    {
                        param.name = (strcmp(str, "JOINT") == 0) ? Param::NAME::JOINT : 0;
                        break;
                    }
                    case 'T':
                    {
                        param.name = (strcmp(str, "TRANSFORM") == 0) ? Param::NAME::TRANSFORM : 0;
                        break;
                    }
                    case 'W':
                    {
                        param.name = (strcmp(str, "WEIGHT") == 0) ? Param::NAME::WEIGHT : 0;
                        break;
                    }
                    default: { break; }
                }
            }

            if(param.name == 0)
            {
                m_status = false;
                printf("invalid parameter name \"%s\"\n", str);
            }
        }

        if(m_status)
        {
            const char* str = type->c_str();

            switch(str[0])
            {
                case 'f':
                {
                    if(strcmp(str, "float") == 0) {
                        param.type = Param::TYPE::FLOAT;
                    } else if(strcmp(str, "float4x4") == 0) {
                        param.type = Param::TYPE::FLOAT4X4;
                    }
                    break;
                }
                case 'n':
                {
                    param.type = (strcmp(str, "name") == 0) ? Param::TYPE::NAME : 0;
                    break;
                }
                default: { break; }
            }

            if(param.type == 0) {
                m_status = false;
                printf("invalid parameter type \"%s\"\n", str);
            }
        }
    }

    void read_accessor(const XML::Node* node, Accessor& accessor)
    {
        accessor.source = find_attribute(node, "source");
        if(m_status)
        {
            const std::string* count = nullptr;
            const std::string* stride = nullptr;

            count = find_attribute(node, "count");
            if(m_status)
            {
                stride = find_attribute(node, "stride");
            }

            if(m_status)
            {
                accessor.count = std::stoi(*count);
                accessor.stride = std::stoi(*stride);
            }
        }

        if(m_status)
        {
            const XML::ChildList* params = find_children(node, "param");
            if(m_status)
            {
                unsigned int num_params = params->size();

                if(num_params <= accessor.params.size())
                {
                    for(unsigned int i = 0; m_status && (i < num_params); i++)
                    {
                        read_param(params->at(i), accessor.params[i]);
                    }
                }
                else
                {
                    m_status = false;
                    printf("too many parameters in accessor\n");
                }
            }
        }
    }

    void read_technique(const XML::Node* node, Technique& technique)
    {
        const XML::Node* accessor = find_child(node, "accessor");
        if(m_status)
        {
            read_accessor(accessor, technique.accessor);
        }
    }

    Source* read_source(const XML::Node* node)
    {
        Source* source = new Source();

        source->id = find_attribute(node, "id");
        if(m_status)
        {
            unsigned int elements_read = 0;
            const XML::Node* array = node->find_child("float_array");
            if(array != nullptr)
            {
                parse_float_array(array->text);
                if(m_status)
                {
                    elements_read = m_float_buffer.size();

                    source->array.type = SourceArray::FLOAT;
                    source->array.float_array = new float[elements_read];
                    memcpy(source->array.float_array, m_float_buffer.data(), sizeof(float) * elements_read);
                }
            }
            else
            {
                array = node->find_child("Name_array");
                if(array != nullptr)
                {
                    parse_name_array(array->text);
                    if(m_status)
                    {
                        elements_read = m_string_buffer.size();

                        source->array.type = SourceArray::NAME;
                        source->array.name_array = new std::string[elements_read];
                        std::copy(m_string_buffer.begin(), m_string_buffer.end(), source->array.name_array);
                    }
                }
                else
                {
                    m_status = false;
                    printf("invalid source array\n");
                }
            }

            if(m_status)
            {
                source->array.id = find_attribute(node, "id");
                if(m_status)
                {
                    const std::string* count = find_attribute(array, "count");
                    if(m_status)
                    {
                        source->array.count = std::stoi(*count);
                    }
                }
            }

            if(m_status)
            {
                if(elements_read != source->array.count)
                {
                    m_status = false;
                    printf("source array length does not match count\n");
                }
            }
        }

        if(m_status)
        {
            const XML::Node* technique = find_child(node, "technique_common");
            if(m_status)
            {
                read_technique(technique, source->technique_common);
            }
        }

        if(!m_status)
        {
            delete source;
            source = nullptr;
        }

        return source;
    }

    void read_triangle_array(const XML::Node* node, TriangleArray* array)
    {
        array->material = find_attribute(node, "material");
        if(m_status)
        {
            const std::string* count = find_attribute(node, "count");
            if(m_status)
            {
                array->count = std::stoi(*count);
            }
        }

        if(m_status)
        {
            const XML::ChildList* inputs = find_children(node, "input");
            if(m_status)
            {
                unsigned int num_inputs = inputs->size();

                if(num_inputs <= array->inputs.size())
                {
                    array->num_inputs = num_inputs;
                    for(unsigned int i = 0; m_status && (i < num_inputs); i++)
                    {
                        read_input(inputs->at(i), array->inputs[i]);
                    }
                }
                else
                {
                    m_status = false;
                    printf("too many inputs in triangle array\n");
                }
            }
        }

        if(m_status)
        {
            const XML::Node* indices = find_child(node, "p");
            if(m_status)
            {
                parse_ushort_array(indices->text);
                if(m_status)
                {
                    unsigned int num_vertices = m_ushort_buffer.size() / array->num_inputs;
                    unsigned int num_triangles = num_vertices / 3;
                    if(num_triangles == array->count)
                    {
                        array->indices = new unsigned short[array->count];
                        memcpy(array->indices, m_ushort_buffer.data(), sizeof(unsigned short) * array->count);
                    }
                    else
                    {
                        m_status = false;
                        printf("triangle index array length does not match count\n");
                    }
                }
            }
        }
    }

    void read_mesh(const XML::Node* node, Mesh& mesh)
    {
        // read the sources
        const XML::ChildList* sources = node->find_children("source");
        if(m_status)
        {
            for(unsigned int i = 0; m_status && (i < sources->size()); i++)
            {
                Source* src = read_source(sources->at(i));
                if(m_status)
                {
                    mesh.sources[*(src->id)] = src;
                }
            }
        }

        // read the vertex array
        if(m_status)
        {
            const XML::Node* vertex_array = find_child(node, "vertices");
            if(m_status)
            {
                read_vertex_array(vertex_array, &mesh.vertices);
                if(m_status)
                {
                    std::string array_source = mesh.vertices.input.source->substr(1);
                    std::map<std::string, Source*>::const_iterator it = mesh.sources.find(array_source);
                    if(it != mesh.sources.end())
                    {
                        mesh.vertices.source = it->second;
                        mesh.sources[*mesh.vertices.id] = it->second;
                    }
                    else
                    {
                        m_status = false;
                        printf("could not find vertex array source \"%s\"\n", array_source.c_str());
                    }
                }
            }
        }

        // read the triangles
        if(m_status)
        {
            const XML::ChildList* triangle_arrays = find_children(node, "triangles");
            if(m_status)
            {
                unsigned int num_arrays = triangle_arrays->size();

                mesh.num_triangle_arrays = num_arrays;
                mesh.triangle_arrays = new TriangleArray[num_arrays];

                for(unsigned int i = 0; m_status && (i < num_arrays); i++)
                {
                    read_triangle_array(triangle_arrays->at(i), &mesh.triangle_arrays[i]);
                }
            }
        }
    }

    void read_joints(const XML::Node* node, Joints& joints)
    {
        const XML::ChildList* inputs = find_children(node, "input");
        if(m_status)
        {
            if(inputs->size() == joints.inputs.size())
            {
                for(unsigned int i = 0; m_status && (i < inputs->size()); i++)
                {
                    read_input(inputs->at(i), joints.inputs[i]);
                }
            }
            else
            {
                m_status = false;
                printf("invalid inputs in joints\n");
            }
        }
    }

    void read_vertex_weights(const XML::Node* node, VertexWeights& weights)
    {
        const std::string* count = find_attribute(node, "count");
        if(m_status)
        {
            weights.count = std::stoi(*count);
        }

        if(m_status)
        {
            const XML::ChildList* inputs = find_children(node, "input");
            if(m_status)
            {
                if(inputs->size() == weights.inputs.size())
                {
                    for(unsigned int i = 0; m_status && (i < inputs->size()); i++)
                    {
                        read_input(inputs->at(i), weights.inputs[i]);
                    }
                }
                else
                {
                    m_status = false;
                    printf("invalid number of inputs in vertex weight set\n");
                }
            }
        }

        if(m_status)
        {
            const XML::Node* vcount = find_child(node, "vcount");
            if(m_status)
            {
                parse_ushort_array(vcount->text);
                if(m_status)
                {
                    if(m_ushort_buffer.size() == weights.count)
                    {
                        weights.v_count_array = m_ushort_buffer;
                    }
                    else
                    {
                        m_status = false;
                        printf("weight count array does not match vertex count\n");
                    }
                }
            }
        }

        if(m_status)
        {
            const XML::Node* v = find_child(node, "v");
            if(m_status)
            {
                parse_ushort_array(v->text);
                if(m_status)
                {
                    weights.v_index_array = m_ushort_buffer;
                }
            }
        }
    }

    void read_skin(const XML::Node* node, Skin& skin)
    {
        // read the source
        const std::string* skin_source = find_attribute(node, "source");
        if(m_status)
        {
            std::string source = skin_source->substr(1);

            std::map<std::string, Geometry*>::const_iterator it = m_data->geometry_library.find(source);
            if(it != m_data->geometry_library.end())
            {
                skin.source = &it->second->mesh;
            }
            else
            {
                m_status = false;
                printf("skin source \"%s\" not found\n", source.c_str());
            }
        }

        // read the bind shape matrix
        if(m_status)
        {
            const XML::Node* bsm = find_child(node, "bind_shape_matrix");
            if(m_status)
            {
                parse_float_array(bsm->text);
                if(m_status)
                {
                    if(m_float_buffer.size() == 16)
                    {
                        memcpy(skin.bind_shape_matrix, m_float_buffer.data(), sizeof(float) * 16);
                    }
                    else
                    {
                        m_status = false;
                        printf("invalid bind shape matrix in skin\n");
                    }
                }
            }
        }

        // read the sources
        if(m_status)
        {
            const XML::ChildList* sources = find_children(node, "source");
            if(m_status)
            {
                for(unsigned int i = 0; m_status && (i < sources->size()); i++)
                {
                    Source* src = read_source(sources->at(i));
                    if(m_status)
                    {
                        skin.sources[*(src->id)] = src;
                    }
                }
            }
        }

        // read the joints
        if(m_status)
        {
            const XML::Node* joints = find_child(node, "joints");
            if(m_status)
            {
                read_joints(joints, skin.joints);
            }

            if(m_status)
            {
                for(unsigned int i = 0; m_status && (i < skin.joints.inputs.size()); i++)
                {
                    Input& input = skin.joints.inputs[i];
                    std::string input_source = input.source->substr(1);

                    const std::map<std::string, Source*>::const_iterator it = skin.sources.find(input_source);
                    if(it != skin.sources.end())
                    {
                        switch(input.semantic)
                        {
                            case Input::JOINT:
                            {
                                skin.joints.joints = it->second;
                                break;
                            }
                            case Input::INV_BIND_MATRIX:
                            {
                                skin.joints.bind_poses = it->second;
                                break;
                            }
                        }
                    }
                    else
                    {
                        m_status = false;
                        printf("could not find joint source \"%s\"\n", input_source.c_str());
                    }
                }
            }
        }
        
        // read the vertex weights
        if(m_status)
        {
            const XML::Node* vertex_weights = find_child(node, "vertex_weights");
            if(m_status)
            {
                read_vertex_weights(vertex_weights, skin.vertex_weights);
            }

            if(m_status)
            {
                for(unsigned int i = 0; m_status && (i < skin.vertex_weights.inputs.size()); i++)
                {
                    Input& input = skin.vertex_weights.inputs[i];
                    std::string input_source = input.source->substr(1);

                    const std::map<std::string, Source*>::const_iterator it = skin.sources.find(input_source);
                    if(it != skin.sources.end())
                    {
                        switch(input.semantic)
                        {
                            case Input::JOINT:
                            {
                                skin.vertex_weights.joints = it->second;
                                break;
                            }
                            case Input::WEIGHT:
                            {
                                skin.vertex_weights.weights = it->second;
                                break;
                            }
                        }
                    }
                    else
                    {
                        m_status = false;
                        printf("could not find vertex weight source \"%s\"\n", input_source.c_str());
                    }
                }
            }
        }
    }

    void read_geometry(const XML::Node* node)
    {
        Geometry* geometry = new Geometry();

        geometry->id = find_attribute(node, "id");
        if(m_status)
        {
            geometry->name = find_attribute(node, "name");
        }

        if(m_status)
        {
            const XML::Node* mesh_node = find_child(node, "mesh");
            if(m_status)
            {
                read_mesh(mesh_node, geometry->mesh);
            }
        }

        if(m_status)
        {
            m_data->geometry_library[*(geometry->id)] = geometry;
        }
        else
        {
            delete geometry;
            m_status = false;
        }
    }

    void read_controller(const XML::Node* node)
    {
        Controller* controller = new Controller();

        controller->id = find_attribute(node, "id");
        if(m_status)
        {
            controller->name = find_attribute(node, "name");
        }

        if(m_status)
        {
            const XML::Node* skin = find_child(node, "skin");
            if(m_status)
            {
                read_skin(skin, controller->skin);
            }
        }

        if(m_status)
        {
            m_data->controller_library[*(controller->id)] = controller;
        }
        else
        {
            delete controller;
            m_status = false;
        }
    }

    void read_geometry_library(const XML::Node* node)
    {
        const XML::ChildList* geometries = find_children(node, "geometry");
        if(m_status)
        {
            for(unsigned int i = 0; m_status && (i < geometries->size()); i++)
            {
                read_geometry(geometries->at(i));
            }
        }
    }

    void read_controller_library(const XML::Node* node)
    {
        const XML::ChildList* controllers = find_children(node, "controller");
        if(m_status)
        {
            for(unsigned int i = 0; m_status && (i < controllers->size()); i++)
            {
                read_controller(controllers->at(i));
            }
        }
    }

    bool process(const XML::Node* root)
    {
        const XML::Node* geometry_library = find_child(root, "library_geometries");
        if(m_status)
        {
            read_geometry_library(geometry_library);
        }

        if(m_status)
        {
            const XML::Node* controller_library = find_child(root, "library_controllers");
            if(m_status)
            {
                read_controller_library(controller_library);
            }
        }
        
        return m_status;
    }

public: // interface
    static Collada* Read(const XML::Node* root)
    {
        Collada* data = new Collada();
        ColladaReader reader(data);

        if(!reader.process(root))
        {
            delete data;
            data = nullptr;
        }

        return data;
    }
};

ColladaImporter::ColladaImporter(const char* path)
{
    bool status = true;
    const XML* xml = XML::Read(path);

    if(xml == nullptr)
    {
        status = false;
        printf("error: could not load xml file\n");
    }
    //xml->root->print();

    status = ColladaReader::Read(xml->root) != nullptr;

    printf("%s\n", status ? "xml loaded successfully" : "xml failed to load");
}

