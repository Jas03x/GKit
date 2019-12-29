#include <gk/collada_importer.hpp>

#include <stdio.h>

#include <gk/string.hpp>
#include <gk/string_buffer.hpp>
#include <gk/xml.hpp>

typedef std::vector<float> FloatArray;
typedef std::vector<std::string*> NameArray;

struct SourceArray
{
    enum TYPE
    {
        NONE  = 0,
        TEXT  = 1,
        FLOAT = 2
    };

    TYPE  type;
    void* data;
};

struct Source
{
    const std::string* id;
    unsigned int stride;
    unsigned int length;

    SourceArray array;
};

struct Mesh
{
    Source* vertex_array;
    std::map<std::string, Source*> sources;
};

struct Geometry
{
    const std::string* id;
    Mesh mesh;
};

int parse_float_array(const std::string& text, std::vector<float>& float_array)
{
    int n = 0;
    StringBuffer buffer;
    
    char c = 0;
    for(unsigned int i = 0 ;; i++)
    {
        c = text[i];

        if(IS_NUM(c) || (c == '.'))
        {
            buffer.push_back(c);
        }
        else if(buffer.size() > 0)
        {
            n++;
            float_array.push_back(buffer.to_float());
        }

        if(c == 0)
        {
            break;
        }
    }

    return n;
}

int parse_name_array(const std::string& text, std::vector<std::string*>& name_array)
{
    int n = 0;
    StringBuffer buffer;

    char c = 0;
    for(unsigned int i = 0 ;; i++)
    {
        c = text[i];

        if(IS_SPACE(c))
        {
            if(buffer.size() > 0)
            {
                name_array.push_back(new std::string(buffer.data(), buffer.size()));
                buffer.clear();
            }
        }
        else
        {
            buffer.push_back(c);
        }

        if(c == 0)
        {
            break;
        }
    }

    return n;
}

bool read_source(const XML::Node* node, Source& src)
{
    printf("read source\n");
    bool status = true;

    src.id = node->find_attribute("id");
    if(src.id != nullptr)
    {
        int elements_read = 0;
        const XML::Node* array = nullptr;
        
        XML::ChildMap::const_iterator it = node->children.find("float_array");
        if(it != node->children.end())
        {
            array = it->second[0];

            FloatArray* float_array = new FloatArray();
            src.array.type = SourceArray::FLOAT;
            src.array.data = float_array;
            elements_read = parse_float_array(array->text, *float_array);
        }
        else
        {
            it = node->children.find("name_array");
            if(it != node->children.end())
            {
                array = it->second[0];

                NameArray* name_array = new NameArray();
                src.array.type = SourceArray::TEXT;
                src.array.data = name_array;
                elements_read = parse_name_array(array->text, *name_array);
            }
            else
            {
                status = false;
                printf("invalid source data\n");
            }
        }
    }

    if(status)
    {
        status = false;

        const XML::Node* common = node->find_child("technique_common");
        if(common != nullptr)
        {
            const XML::Node* accessor = common->find_child("accessor");
            if(accessor != nullptr)
            {
                const std::string* length = accessor->find_attribute("count");
                const std::string* stride = accessor->find_attribute("stride");

                if((length != nullptr) && (stride != nullptr))
                {
                    status = true;
                    src.length = std::stoi(*length);
                    src.stride = std::stoi(*stride);
                }
            }
        }
    }

    return status;
}

bool read_mesh(const XML::Node* node, Mesh& mesh)
{
    printf("read mesh\n");
    bool status = true;

    const XML::ChildList* sources = node->find_children("source");
    for(unsigned int i = 0; status && (i < sources->size()); i++)
    {
        Source* src = new Source();
        if(read_source(sources->at(i), *src))
        {
            mesh.sources[*src->id] = src;
        }
        else
        {
            status = false;
            delete src;
        }
    }

    // read the vertex array
    if(status)
    {
        status = false; // false unless object successfully read
        
        const XML::Node* vertex_array = node->find_child("vertices");
        if(vertex_array != nullptr)
        {
            const std::string* id = vertex_array->find_attribute("id");
            const XML::Node* input = vertex_array->find_child("input");
            if(input != nullptr)
            {
                const std::string* source = input->find_attribute("source");
                if(source != nullptr)
                {
                    std::map<std::string, Source*>::const_iterator it = mesh.sources.find(source->substr(1));
                    if(it != mesh.sources.end())
                    {
                        status = true;
                        mesh.vertex_array = it->second;
                        mesh.sources[*id] = mesh.vertex_array;
                    }
                    else
                    {
                        printf("error: could not find vertex array source \"%s\"\n", source->c_str());
                    }
                }
            }
        }
    }

    // read the triangle array

    return status;
}

bool read_geometry_library(const XML::Node* node, std::vector<Geometry>& library)
{
    printf("read geometry library\n");
    bool status = true;

    const XML::ChildList* geometry_list = node->find_children("geometry");
    for(unsigned int i = 0; status && (i < geometry_list->size()); i++)
    {
        status = false; // false unless object successfully read
        library.push_back(Geometry());

        Geometry& src = library.back();
        const XML::Node* geometry_node = geometry_list->at(i);

        src.id = geometry_node->find_attribute("id");
        if(src.id != nullptr)
        {
            const XML::Node* mesh = geometry_node->find_child("mesh");
            if(mesh != nullptr)
            {
                if(src.id != nullptr)
                {
                    status = read_mesh(mesh, src.mesh);
                }
            }
        }
    }

    return status;
}

ColladaImporter::ColladaImporter(const char* path)
{
    const XML* xml = XML::Read(path);
    //xml->root->print();

    const XML::Node* collada = xml->root;
    
    std::vector<Geometry> geometry_list;

    const XML::Node* geometry_library = collada->find_child("library_geometries");

    read_geometry_library(geometry_library, geometry_list);
}

