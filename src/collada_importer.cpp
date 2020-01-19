#include <gk/collada_importer.hpp>

#include <utility>

#include <gk/collada_parser.hpp>

Collada::Importer::Importer()
{
}

Collada::Importer::~Importer()
{
}

bool Collada::Importer::Import(const char* path, MeshData& mesh_data)
{
    Parser parser;
    Importer importer;

    bool status = parser.parse(path);
    if(status)
    {
        status = importer.process_scene_library(parser.GetSceneLibrary(), mesh_data);
    }

    if(status)
    {
        status = importer.process_geometry_library(parser.GetGeometryLibrary());
    }

    if(status)
    {
        status = importer.process_controller_library(parser.GetControllerLibrary(), mesh_data);
    }

    if(status)
    {
        status = importer.process_mesh_data(mesh_data);
    }

    return status;
}

bool Collada::Importer::process_geometry_library(const Collada::Parser::GeometryLibrary& library)
{
    bool status = true;

    for(Parser::GeometryLibrary::const_iterator it = library.begin(); it != library.end(); it++)
    {
        process_geometry(it->second);
    }

    return status;
}

bool Collada::Importer::process_controller_library(const Collada::Parser::ControllerLibrary& library, MeshData& mesh_data)
{
    bool status = true;

    if(library.size() != 1)
    {
        status = false;
        printf("error importing collada object: unsupported number of controllers\n");
    }

    const Controller* controller = library.begin()->second;
    const Skin& skin = controller->skin;
    IMesh& source = m_mesh_map.at(controller->skin.source->substr(1));

    mesh_data.bind_pose_matrix = Matrix4F(skin.bind_shape_matrix);

    const SourceArray& bone_names = skin.joints.names->array;
    const SourceArray& bone_offset_matrices = skin.joints.bind_poses->array;
    for(unsigned int i = 0; i < bone_names.count; i++)
    {
        MeshData::Bone& bone = *mesh_data.bones.insert(mesh_data.bones.end(), MeshData::Bone());

        bone.name = bone_names.name_array[i];
        bone.offset_matrix = Matrix4F(&bone_offset_matrices.float_array[i * 16]);
    }

    unsigned int joint_offset = 0;
    unsigned int weight_offset = 0;
    for(unsigned int i = 0; status && (i < skin.vertex_weights.inputs.size()); i++)
    {
        const Input& input = skin.vertex_weights.inputs[i];
        switch(input.semantic)
        {
            case Input::JOINT:  { joint_offset  = input.offset; break; }
            case Input::WEIGHT: { weight_offset = input.offset; break; }
            default:
            {
                status = false;
                printf("unknown mesh input semantic\n");
            }
        }
    }

    const VertexWeights& vertex_weights = skin.vertex_weights;
    const float* v_weights = vertex_weights.weights->array.float_array;
    const unsigned short* v_indices = vertex_weights.v_index_array.data();

    for(unsigned int i = 0; i < vertex_weights.count; i++)
    {
        unsigned int count = vertex_weights.v_count_array[i];
        source.vertex_array[i].bone_count = count;
        
        for(unsigned int j = 0; j < count; j++)
        {
            source.vertex_array[i].bone_indices[j] = *v_indices;
            source.vertex_array[i].bone_weights[j] = v_weights[*(v_indices + 1)];
            v_indices += 2;
        }
    }

    return status;
}

bool Collada::Importer::process_scene_library(const Collada::Parser::SceneLibrary& library, MeshData& mesh_data)
{
    bool status = true;

    if(library.size() != 1)
    {
        status = false;
        printf("error importing collada object: unsupported number of scenes\n");
    }

    const VisualScene* scene = library.begin()->second;
    for(std::map<std::string, Node*>::const_iterator it = scene->nodes.begin(); status && (it != scene->nodes.end()); it++)
    {
        status = process_node(it->second, mesh_data);
    }

    return status;
}

bool Collada::Importer::process_node(const Collada::Node* node, MeshData& mesh_data)
{
    bool status = true;

    MeshData::Node& n = *mesh_data.nodes.insert(mesh_data.nodes.end(), MeshData::Node());
    n.name = *node->name;
    n.parent = node->parent == nullptr ? nullptr : *node->parent->name;
    n.scale = Vector3F(node->scale[0], node->scale[1], node->scale[2]);
    n.rotation = Vector3F(node->rotation_x[3], node->rotation_y[3], node->rotation_z[3]);
    n.translation = Vector3F(node->translation[0], node->translation[1], node->translation[2]);

    m_node_map[n.name] = m_node_map.size();

    return status;
}

bool Collada::Importer::process_geometry(const Geometry* obj)
{
    bool status = true;
    IMesh& data = m_mesh_map.insert(std::pair<std::string, IMesh>(*obj->id, IMesh())).second;

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

    for(unsigned int i = 0; i < vertex_source->count; i++)
    {
        const float* array = vertex_source->float_array;
        unsigned int offset = i * 3;
        
        IVertex& vertex = *data.vertex_array.insert(data.vertex_array.end(), IVertex());
        vertex.position = Vector3F(array[offset + 0], array[offset + 1], array[offset + 2]);
    }

    for(unsigned int i = 0; i < normal_source->count; i++)
    {
        const float* array = normal_source->float_array;
        unsigned int offset = i * 3;
        data.normal_array.push_back(Vector3F(array[offset + 0], array[offset + 1], array[offset + 2]));
    }

    for(unsigned int i = 0; i < uv_source->count; i++)
    {
        const float* array = uv_source->float_array;
        unsigned int offset = i * 2;
        data.uv_array.push_back(Vector2F(array[offset + 0], array[offset + 1]));
    }

    data.node_index = m_node_map[*obj->name];
    for(unsigned int i = 0; i < triangle_array.count; i += triangle_array.num_inputs)
    {
        data.indices.push_back(i + vertex_offset);
        data.indices.push_back(i + normal_offset);
        data.indices.push_back(i + uv_offset);
    }

    return status;
}

bool Collada::Importer::process_mesh_data(const MeshData& mesh_data)
{
    bool status = true;

    // todo:
    // process all vertices into a "vertex array"
    // then index all the meshes into the vertex_array

    for(std::map<std::string, IMesh>::const_iterator it = m_mesh_map.begin(); it != m_mesh_map.end(); it++)
    {
        IMesh& mesh = it->second;
    }

    return status;
}
