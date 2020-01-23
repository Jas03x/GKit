#include <gk/collada_parser.hpp>

#include <cstdio>
#include <cstring>

#include <algorithm>
#include <array>
#include <vector>

const XML::Node* Collada::Parser::find_child(const XML::Node* node, const std::string& name)
{
    const XML::Node* child = node->find_child(name);
    if(child == nullptr)
    {
        m_status = false;
        printf("error: could not find child \"%s\"\n", name.c_str());
    }
    return child;
}

const XML::ChildList* Collada::Parser::find_children(const XML::Node* node, const std::string& name)
{
    const XML::ChildList* children = node->find_children(name);
    if(children == nullptr)
    {
        m_status = false;
        printf("error: could not find children \"%s\"\n", name.c_str());
    }
    return children;
}

const std::string* Collada::Parser::find_attribute(const XML::Node* node, const std::string& name)
{
    const std::string* attribute = node->find_attribute(name);
    if(attribute == nullptr)
    {
        m_status = false;
        printf("error: could not find attribute \"%s\"\n", name.c_str());
    }
    return attribute;
}

void Collada::Parser::parse_float_array(const std::string& text)
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

void Collada::Parser::parse_ushort_array(const std::string& text)
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

void Collada::Parser::parse_name_array(const std::string& text)
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

void Collada::Parser::read_input(const XML::Node* node, Input& input)
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

void Collada::Parser::read_vertex_array(const XML::Node* node, VertexArray* array)
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

void Collada::Parser::read_param(const XML::Node* node, Param& param)
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

void Collada::Parser::read_accessor(const XML::Node* node, Accessor& accessor)
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

void Collada::Parser::read_technique(const XML::Node* node, Technique& technique)
{
    const XML::Node* accessor = find_child(node, "accessor");
    if(m_status)
    {
        read_accessor(accessor, technique.accessor);
    }
}

Collada::Source* Collada::Parser::read_source(const XML::Node* node)
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

void Collada::Parser::read_triangle_array(const XML::Node* node, TriangleArray* array)
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

void Collada::Parser::read_mesh(const XML::Node* node, Mesh& mesh)
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

void Collada::Parser::read_joints(const XML::Node* node, Joints& joints)
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

void Collada::Parser::read_vertex_weights(const XML::Node* node, VertexWeights& weights)
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

void Collada::Parser::read_skin(const XML::Node* node, Skin& skin)
{
    // read the source
    const std::string* skin_source = find_attribute(node, "source");
    if(m_status)
    {
        /*
        std::string source = skin_source->substr(1);

        std::map<std::string, Geometry*>::const_iterator it = m_geometry_library.find(source);
        if(it != m_geometry_library.end())
        {
            skin.source = &it->second->mesh;
        }
        else
        {
            m_status = false;
            printf("skin source \"%s\" not found\n", source.c_str());
        }
        */
       skin.source = skin_source;
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
                            skin.joints.names = it->second;
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

void Collada::Parser::read_geometry(const XML::Node* node)
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
        m_geometry_library[*(geometry->id)] = geometry;
    }
    else
    {
        delete geometry;
    }
}

void Collada::Parser::read_controller(const XML::Node* node)
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
        m_controller_library[*(controller->id)] = controller;
    }
    else
    {
        delete controller;
    }
}

void Collada::Parser::read_extra_data(const XML::Node* node, Extra& extra)
{
    const XML::Node* tech = find_child(node, "technique");
    if(m_status)
    {
        extra.technique.profile = find_attribute(tech, "profile");
    }

    if(m_status)
    {
        const XML::Node* layer = find_child(tech, "layer");
        if(m_status)
        {
            extra.technique.layer = &layer->text;
        }
    }

    if(m_status)
    {
        const XML::Node* roll = tech->find_child("roll");
        if(roll != nullptr)
        {
            extra.technique.roll = std::stof(roll->text);
        }
    }

    if(m_status)
    {
        const XML::Node* tip_x = find_child(tech, "tip_x");
        if(m_status)
        {
            extra.technique.tip_x = std::stoi(tip_x->text);
        }
    }
    
    if(m_status)
    {
        const XML::Node* tip_y = find_child(tech, "tip_y");
        if(m_status)
        {
            extra.technique.tip_y = std::stoi(tip_y->text);
        }
    }
    
    if(m_status)
    {
        const XML::Node* tip_z = find_child(tech, "tip_z");
        if(m_status)
        {
            extra.technique.tip_z = std::stoi(tip_z->text);
        }
    }
}

void Collada::Parser::read_node(const XML::Node* node, Node* parent, VisualScene* scene)
{
    Node* data = new Node();

    data->id = find_attribute(node, "id");
    if(m_status)
    {
        data->name = find_attribute(node, "name");
    }

    if(m_status)
    {
        const std::string* type = find_attribute(node, "type");
        if(m_status)
        {
            const char* str = type->c_str();
            switch(str[0])
            {
                case 'N': { data->type = (strcmp(str, "NODE") == 0) ? Node::NODE : 0; break; }
                case 'J': { data->type = (strcmp(str, "JOINT") == 0) ? Node::JOINT : 0; break; }
            }

            if(data->type == 0)
            {
                m_status = false;
                printf("invalid node type\n");
            }
        }
    }

    if(m_status)
    {
        data->sid = node->find_attribute("sid");
    }

    if(m_status)
    {
        const XML::Node* scale = find_child(node, "scale");
        if(m_status)
        {
            parse_float_array(scale->text);
            if(m_status)
            {
                if(m_float_buffer.size() == 3)
                {
                    memcpy(data->scale, m_float_buffer.data(), sizeof(float) * 3);
                }
                else
                {
                    m_status = false;
                    printf("invalid scale parameters\n");
                }
            }
        }
    }

    if(m_status)
    {
        const XML::ChildList* rotations = find_children(node, "rotate");
        if(m_status)
        {
            if(rotations->size() == 3)
            {
                for(unsigned int i = 0; m_status && (i < rotations->size()); i++)
                {
                    const XML::Node* rotation = rotations->at(i);
                    
                    const std::string* sid = find_attribute(rotation, "sid");
                    if(m_status)
                    {
                        parse_float_array(rotation->text);
                        if(m_status)
                        {
                            if(m_float_buffer.size() == 4)
                            {
                                float* r_data = nullptr;

                                const char* sid_str = sid->c_str();
                                if(strncmp(sid_str, "rotation", 8) == 0)
                                {
                                    switch(sid_str[8])
                                    {
                                        case 'X': { r_data = data->rotation_x; break; }
                                        case 'Y': { r_data = data->rotation_y; break; }
                                        case 'Z': { r_data = data->rotation_z; break; }
                                        default: { break; }
                                    }
                                }

                                if(r_data != nullptr)
                                {
                                    memcpy(r_data, m_float_buffer.data(), sizeof(float) * 4);
                                }
                                else
                                {
                                    m_status = false;
                                    printf("unknown rotation \"%s\"\n", sid_str);
                                }
                            }
                            else
                            {
                                m_status = false;
                                printf("invalid rotation parameters\n");
                            }
                        }
                    }
                }
            }
            else
            {
                m_status = false;
                printf("expected three rotation axis\n");
            }
        }
    }
    
    if(m_status)
    {
        const XML::Node* translation = find_child(node, "translate");
        if(m_status)
        {
            parse_float_array(translation->text);
            if(m_status)
            {
                if(m_float_buffer.size() == 3)
                {
                    memcpy(data->translation, m_float_buffer.data(), sizeof(float) * 3);
                }
                else
                {
                    m_status = false;
                    printf("invalid translation parameters\n");
                }
            }
        }
    }

    if(m_status)
    {
        const XML::Node* extra_data = node->find_child("extra");
        if(extra_data != nullptr)
        {
            read_extra_data(extra_data, data->extra);
        }
    }

    if(m_status)
    {
        data->parent = parent;
        scene->nodes[*(data->id)] = data;
    }
    else
    {
        delete data;
    }

    if(m_status)
    {
        const XML::ChildList* children = node->find_children("node");
        if(children != nullptr)
        {
            for(unsigned int i = 0; m_status && (i < children->size()); i++)
            {
                read_node(children->at(i), data, scene);
            }
        }
    }
}

void Collada::Parser::read_visual_scene(const XML::Node* node)
{
    VisualScene* scene = new VisualScene();

    scene->id = find_attribute(node, "id");
    if(m_status)
    {
        scene->name = find_attribute(node, "name");
    }

    if(m_status)
    {
        const XML::ChildList* nodes = node->find_children("node");
        if(nodes != nullptr)
        {
            for(unsigned int i = 0; m_status && (i < nodes->size()); i++)
            {
                read_node(nodes->at(i), nullptr, scene);
            }
        }
    }

    if(m_status)
    {
        m_scene_library[*(scene->id)] = scene;
    }
    else
    {
        delete scene;
    }
}

void Collada::Parser::read_image(const XML::Node* node)
{
    Image* image = new Image();

    image->id = find_attribute(node, "id");
    if(m_status)
    {
        image->name = find_attribute(node, "name");
    }

    if(m_status)
    {
        const XML::Node* init_data = find_child(node, "init_from");
        if(m_status)
        {
            image->init_from.file_name = &init_data->text;
        }
    }

    if(m_status)
    {
        m_image_library[*(image->id)] = image;
    }
    else
    {
        delete image;
    }
}

void Collada::Parser::read_geometry_library(const XML::Node* node)
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

void Collada::Parser::read_controller_library(const XML::Node* node)
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

void Collada::Parser::read_visual_scene_library(const XML::Node* node)
{
    const XML::Node* visual_scene = find_child(node, "visual_scene");
    if(m_status)
    {
        read_visual_scene(visual_scene);
    }
}

void Collada::Parser::read_image_library(const XML::Node* node)
{
    const XML::ChildList* images = find_children(node, "image");
    if(m_status)
    {
        for(unsigned int i = 0; m_status && (i < images->size()); i++)
        {
            read_image(images->at(i));
        }
    }
}

void Collada::Parser::process(const XML::Node* root)
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

    if(m_status)
    {
        const XML::Node* visual_scene_library = find_child(root, "library_visual_scenes");
        if(m_status)
        {
            read_visual_scene_library(visual_scene_library);
        }
    }

    if(m_status)
    {
        const XML::Node* image_library = find_child(root, "library_images");
        if(m_status)
        {
            read_image_library(image_library);
        }
    }
}

Collada::Parser::Parser()
{
}

Collada::Parser::~Parser()
{
    for(GeometryLibrary::const_iterator it = m_geometry_library.begin(); it != m_geometry_library.end(); it++)
    {
        delete it->second;
    }

    for(ControllerLibrary::const_iterator it = m_controller_library.begin(); it != m_controller_library.end(); it++)
    {
        delete it->second;
    }

    for(SceneLibrary::const_iterator it = m_scene_library.begin(); it != m_scene_library.end(); it++)
    {
        delete it->second;
    }

    for(ImageLibrary::const_iterator it = m_image_library.begin(); it != m_image_library.end(); it++)
    {
        delete it->second;
    }

    delete m_file;
}

bool Collada::Parser::parse(const char* path)
{
    m_status = true;
    m_file = XML::Read(path);

    if(m_file != nullptr)
    {
        //m_file->root->print();
        process(m_file->root);
        printf("%s\n", m_status ? "xml loaded successfully" : "xml failed to load");
    }
    else
    {
        m_status = false;
        printf("error: could not load xml file\n");
    }

    return m_status;
}

const Collada::Parser::GeometryLibrary& Collada::Parser::GetGeometryLibrary()
{
    return m_geometry_library;
}

const Collada::Parser::ControllerLibrary& Collada::Parser::GetControllerLibrary()
{
    return m_controller_library;
}

const Collada::Parser::SceneLibrary& Collada::Parser::GetSceneLibrary()
{
    return m_scene_library;
}

const Collada::Parser::ImageLibrary& Collada::Parser::GetImageLibrary()
{
    return m_image_library;
}
