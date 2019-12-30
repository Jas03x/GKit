#include <gk/collada_importer.hpp>

#include <cstdio>
#include <cstring>

#include <gk/array.hpp>
#include <gk/const_vector.hpp>
#include <gk/string.hpp>
#include <gk/string_buffer.hpp>
#include <gk/xml.hpp>

struct FloatArray 
{
    const std::string* id;
    unsigned int count;
    float* data;
};

struct NameArray
{
    const std::string* id;
    unsigned int count;
    const char** data;
};

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
    
    ConstVector<Param, 4> params;
};

struct Technique
{
    Accessor accessor;
};

struct Source
{
    const std::string* id;
    void* array;

    Technique technique_common;
};

struct Input
{
    const std::string* source;
    unsigned int offset;
    unsigned int set;

    enum
    {
        POSITION = 1
    } semantic;
};

struct VertexArray
{
    const std::string* id;
    Input* input;
    Source* source;
};

struct TriangleArray
{
    const std::string* material;
    unsigned int count;
    unsigned short* indices;
    
    ConstVector<Input, 4> inputs;
};

struct Mesh
{
    std::map<std::string, Source*> sources;

    VertexArray* vertices;
    Array<TriangleArray*> triangle_arrays;
};

struct Geometry
{
    const std::string* id;
    const std::string* name;

    Mesh* mesh;
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
        const CML::ChildList* children = node->find_children(name);
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

    Mesh* read_mesh(const XML::Node* node)
    {
        return nullptr;
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

