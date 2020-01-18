#include <gk/collada_importer.hpp>

#include <gk/collada_parser.hpp>

Collada::Importer::Importer()
{
}

Collada::Importer::~Importer()
{
}

bool Collada::Importer::Import(const char* path, MeshData& data)
{
    Parser parser;
    Importer importer;

    bool status = parser.parse(path);
    if(status)
    {
        status = importer.process_scene_library(parser.GetSceneLibrary(), data);
    }

    if(status)
    {
        status = importer.process_geometry_library(parser.GetGeometryLibrary(), data);
    }

    return status;
}

bool Collada::Importer::process_geometry_library(const Collada::Parser::GeometryLibrary& library, MeshData& data)
{
    bool status = true;

    for(Parser::GeometryLibrary::const_iterator it = library.begin(); it != library.end(); it++)
    {
        process_geometry(it->second, data);
    }

    return status;
}

bool Collada::Importer::process_controller_library(const Collada::Parser::ControllerLibrary& library, MeshData& data)
{
    bool status = true;

    return status;
}

bool Collada::Importer::process_scene_library(const Collada::Parser::SceneLibrary& library, MeshData& data)
{
    bool status = true;

    if(library.size() != 1)
    {
        printf("error importing collada object: unsupported number of scenes\n");
        status = false;
    }

    const VisualScene* scene = library.begin()->second;
    for(std::map<std::string, Node*>::const_iterator it = scene->nodes.begin(); status && (it != scene->nodes.end()); it++)
    {
        status = process_node(it->second, data);
    }

    return status;
}

bool Collada::Importer::process_node(const Collada::Node* node, MeshData& data)
{
    bool status = true;
    data.nodes.push_back(MeshData::Node());
    
    MeshData::Node& n = data.nodes.back();
    n.name = *node->name;
    n.parent = node->parent == nullptr ? nullptr : *node->parent->name;
    n.scale = Vector3F(node->scale[0], node->scale[1], node->scale[2]);
    n.rotation = Vector3F(node->rotation_x[3], node->rotation_y[3], node->rotation_z[3]);
    n.translation = Vector3F(node->translation[0], node->translation[1], node->translation[2]);

    m_node_map[n.name] = m_node_map.size();

    return status;
}

bool Collada::Importer::process_geometry(const Geometry* obj, MeshData& data)
{
    bool status = true;

    const Mesh& mesh = obj->mesh;
    const TriangleArray& triangle_array = *mesh.triangle_arrays;

    const SourceArray* vertex_source = nullptr;
    const SourceArray* normal_source = nullptr;
    const SourceArray* uv_source = nullptr;

    unsigned int vertex_offset = 0;
    unsigned int normal_offset = 0;
    unsigned int uv_offset = 0;

    for(unsigned int i = 0; status && (i < triangle_array.num_inputs); i++)
    {
        const Input& input = triangle_array.inputs[i];

        const std::map<std::string, Source*>::const_iterator it = mesh.sources.find(*input.source);
        if(it != mesh.sources.end())
        {
            switch(input.semantic)
            {
                case Input::VERTEX:   { vertex_source = &it->second->array; vertex_offset = input.offset; break; }
                case Input::NORMAL:   { normal_source = &it->second->array; normal_offset = input.offset; break; }
                case Input::TEXCOORD: { uv_source = &it->second->array; uv_offset = input.offset; break; }
                case Input::COLOR:    { break; }
                default:
                {
                    status = false;
                    printf("unknown mesh input semantic\n");
                    break;
                }
            }
        }
        else
        {
            status = false;
            printf("error: source \"%s\" not found\n", input.source->c_str());
        }
    }

    std::vector<Vector3F> vertex_array;
    std::vector<Vector3F> normal_array;
    std::vector<Vector2F> uv_array;

    for(unsigned int i = 0; i < vertex_source->count; i++)
    {
        const float* array = vertex_source->float_array;
        unsigned int offset = i * 3;
        vertex_array.push_back(Vector3F(array[offset + 0], array[offset + 1], array[offset + 2]));
    }

    for(unsigned int i = 0; i < normal_source->count; i++)
    {
        const float* array = normal_source->float_array;
        unsigned int offset = i * 3;
        normal_array.push_back(Vector3F(array[offset + 0], array[offset + 1], array[offset + 2]));
    }

    for(unsigned int i = 0; i < uv_source->count; i++)
    {
        const float* array = uv_source->float_array;
        unsigned int offset = i * 2;
        uv_array.push_back(Vector2F(array[offset + 0], array[offset + 1]));
    }

    unsigned int node_index = m_node_map[*obj->name];
    for(unsigned int i = 0; i < triangle_array.count; i += triangle_array.num_inputs)
    {
        data.vertices.push_back(MeshData::Vertex());
        
        MeshData::Vertex& vertex = data.vertices.back();
        vertex.position = vertex_array[i + vertex_offset];
        vertex.normal = normal_array[i + normal_offset];
        vertex.uv = uv_array[i + uv_offset];
        vertex.node = node_index;
    }

    return status;
}
