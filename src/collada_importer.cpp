#include <gk/collada_importer.hpp>

#include <cstdio>
#include <cstring>

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
        Z = 3
    } name;

    enum TYPE
    {
        FLOAT = 1
    } type;
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
    const std::string* id;
    unsigned int count;

    union
    {
        FLOAT = 1,
        NAME  = 2
    } type;

    union
    {
        float* float_array;
        const char** name_array;
    } array;
};

struct Source
{
    const std::string* id;
    SourceArray array;
    Technique technique_common;
};

struct Input
{
    const std::string* source;
    unsigned int offset;
    unsigned int set;

    enum
    {
        POSITION = 1,
        VERTEX = 2,
        NORMAL = 3,
        TEXCOORD = 4,
        COLOR = 5
    } semantic;
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

struct Geometry
{
    const std::string* id;
    const std::string* name;

    Mesh mesh;
};

struct Collada
{
    std::map<std::string, Geometry*> geometry_library;
};

class ColladaReader
{
private: // data
    bool m_status;
    Collada* m_data;

    StringBuffer m_string_buffer;

    std::vector<float> m_float_buffer;
    std::vector<unsigned short> m_ushort_buffer;
    std::vector<char*> m_cstring_buffer;

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

    void read_input(const XML::Node* node, Input* input)
    {
        const std::string* semantic = nullptr;

        semantic = find_attribute(node, "semantic");
        if(m_status)
        {
            input->source = find_attribute(node, "source");
        }

        if(m_status)
        {
            unsigned int type = 0;

            const char* c_str = semantic->c_str();
            switch(c_str[0])
            {
                case 'C':
                {
                    type = (strncmp(c_str, "COLOR") == 0) ? Input::COLOR : 0;
                    break;
                }
                case 'N':
                {
                    type = (strncmp(c_str, "NORMAL") == 0) ? Input::NORMAL : 0;
                    break;
                }
                case 'P':
                {
                    type = (strncmp(c_str, "POSITION") == 0) ? Input::POSITION : 0;
                    break;
                }
                case 'T':
                {
                    type = (strncmp(c_str, "TEXCOORD") == 0) ? Input::TEXCOORD : 0;
                    break;
                }
                case 'V':
                {
                    type = (strncmp(c_str, "VERTEX") == 0) ? Input::VERTEX : 0;
                    break;
                }
                default: { break; }
            }

            if(type != 0)
            {
                input->semantic = type;
            }
            else
            {
                m_status = false;
                printf("invalid semantic: \"%s\"\n", semantic->c_str());
            }
        }

        if(m_status)
        {
            const std::string* offset = node->find_child("offset");
            if(offset != nullptr) {
                input->offset = std::stoi(*offset);
            }

            const std::string* set = node->find_child("set");
            if(set != nullptr) {
                input->set = std::stoi(*set);
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
                read_input(input, &array->input);
            }
        }
    }

    Source* read_source(const XML::Node* node)
    {
        Source* source = new Source();

        source->id = find_attribute(node, "id");
        if(m_status)
        {
            source->name = find_attribute(node, "name");
        }

        if(m_status)
        {
            const XML::Node* array = node->find_child("float_array");
            if(array != nullptr)
            {
                parse_float_array(array, &source->array);
            }
            else
            {
                array = node->find_child("name_array");
                if(array != nullptr)
                {
                    parse_name_array(array, &source->array);
                }
                else
                {
                    m_status = false;
                    printf("invalid source array\n");
                }
            }
        }

        if(m_status)
        {
            const XML::Node* technique = find_node(node, "technique_common");
            if(m_status)
            {
                parse_technique(technique, &source->technique_common);
            }
        }

        if(!m_status)
        {
            delete source;
            source = nullptr;
        }

        return source;
    }

    Mesh* read_mesh(const XML::Node* node)
    {
        Mesh* mesh = new Mesh();

        // read the sources
        const XML::ChildList* sources = node->find_children("source");
        if(m_status)
        {
            for(unsigned int i = 0; m_status && (i < sources->size()); i++)
            {
                Source* src = read_source(sources->at(i));
                if(m_status)
                {
                    mesh->sources[*(src->id)] = src;
                }
            }
        }

        // read the vertex array
        if(m_status)
        {
            const XML::Node* vertex_array = find_child(node, "vertices");
            if(m_status)
            {
                read_vertex_array(vertex_array, &mesh->vertices);
                if(m_status)
                {
                    std::map<std::string, Source*>::const_iterator it = mesh->sources.find(*(vertex_array->id));
                    if(it != mesh->sources.end())
                    {
                        mesh->vertices->source = it->second;
                    }
                    else
                    {
                        m_status = false;
                        printf("could not find vertex array source \"%s\"\n", vertex_array->id->c_str());
                    }
                }
            }
        }

        // read the triangles
        if(status)
        {
        }

        if(!m_status)
        {
            delete mesh;
            mesh = nullptr;
        }

        return mesh;
    }

    void read_geometry(const XML::Node* node)
    {
        Mesh* mesh = nullptr;
        const std::string* id = nullptr;
        const std::string* name = nullptr;

        id = find_attribute(node, "id");
        if(m_status)
        {
            name = find_attribute(node, "name");
        }

        if(m_status)
        {
            const XML::Node* mesh_node = find_child(node, "mesh");
            if(m_status)
            {
                mesh = read_mesh(mesh_node);
            }
        }

        if(m_status)
        {
            Geometry* geometry = new Geometry();
            geometry->id = id;
            geometry->name = name;
            geometry->mesh = mesh;

            m_data->geometry_library[*id] = geometry;
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

    bool process(const XML::Node* root)
    {
        const XML::Node* geometry_library = find_child(root, "geometry_library");
        if(m_status)
        {
            read_geometry_library(geometry_library);
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

