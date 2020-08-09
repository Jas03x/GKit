#include <gk/collada_importer.hpp>

#include <math.h>

#include <utility>

#include <gk/collada_parser.hpp>
#include <gk/filesystem.hpp>

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

    if (status)
    {
        status = importer.process_asset_info(parser.GetAssetInfo(), mesh_data);
    }

    if (status)
    {
        status = importer.process_scene_library(parser.GetSceneLibrary(), mesh_data);
    }

    if (status)
    {
        status = importer.process_geometry_library(parser.GetGeometryLibrary());
    }

    if (status)
    {
        status = importer.process_controller_library(parser.GetControllerLibrary(), mesh_data);
    }

    if (status)
    {
        status = importer.process_image_library(parser.GetImageLibrary(), mesh_data);
    }

    if (status)
    {
        status = importer.process_mesh_data(mesh_data);
    }

    return status;
}

bool Collada::Importer::process_asset_info(const Collada::AssetInfo& info, MeshData& mesh_data)
{
    bool status = true;

    switch (info.orientation)
    {
        case AssetInfo::Orientation::AXIS_UP:
        {
            mesh_data.orientation = MeshData::Orientation::Z_UP;
            break;
        }

        default:
        {
            status = false;
            printf("unknown orientation\n");
            break;
        }
    }

    return status;
}

bool Collada::Importer::process_geometry_library(const Collada::Parser::GeometryLibrary& library)
{
    bool status = true;

    for (Parser::GeometryLibrary::const_iterator it = library.begin(); it != library.end(); it++)
    {
        process_geometry(it->second);
    }

    return status;
}

bool Collada::Importer::process_controller_library(const Collada::Parser::ControllerLibrary& library, MeshData& mesh_data)
{
    bool status = true;

    unsigned long num_controllers = library.size();
    if (num_controllers > 1)
    {
        status = false;
        printf("error importing collada object: unsupported number of controllers\n");
    }
    else if (num_controllers == 1)
    {
        const Controller* controller = library.begin()->second;
        const Skin& skin = controller->skin;
        IMesh& source = m_mesh_map.at(controller->skin.source->substr(1));

        Matrix4F bind_shape_matrix = Matrix::Transpose(Matrix4F(skin.bind_shape_matrix));
        //printf("bind shape matrix:\n");
        //bind_shape_matrix.print();
        //printf("\n");

        const SourceArray& bone_names = skin.joints.names->array;
        const SourceArray& bone_offset_matrices = skin.joints.bind_poses->array;
        for (unsigned int i = 0; i < bone_names.count; i++)
        {
            MeshData::Bone& bone = *mesh_data.bones.insert(mesh_data.bones.end(), MeshData::Bone());

            //printf("Bone %s:\n", bone_names.name_array[i].c_str());
            //Matrix::Transpose(Matrix4F(&bone_offset_matrices.float_array[i * 16])).print();
            //printf("\n");

            bone.name = bone_names.name_array[i];
            bone.bind_pose_matrix = Matrix::Transpose(Matrix4F(&bone_offset_matrices.float_array[i * 16])) * bind_shape_matrix;
        }

        unsigned int joint_offset = 0;
        unsigned int weight_offset = 0;
        for (unsigned int i = 0; status && (i < skin.vertex_weights.inputs.size()); i++)
        {
            const Input& input = skin.vertex_weights.inputs[i];
            switch (input.semantic)
            {
            case Input::JOINT: { joint_offset = input.offset; break; }
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

        for (unsigned int i = 0; status && (i < vertex_weights.count); i++)
        {
            unsigned int count = vertex_weights.v_count_array[i];

            if (count <= 4)
            {
                source.vertex_array[i].bone_count = count;

                for (unsigned int j = 0; j < count; j++)
                {
                    source.vertex_array[i].bone_indices[j] = v_indices[0];
                    source.vertex_array[i].bone_weights[j] = v_weights[v_indices[1]];
                    v_indices += 2;
                }
            }
            else
            {
                printf("error: more than 4 bones in vertex\n");
            }
        }
    }

    return status;
}

bool Collada::Importer::process_scene_library(const Collada::Parser::SceneLibrary& library, MeshData& mesh_data)
{
    bool status = true;

    if (library.size() != 1)
    {
        status = false;
        printf("error importing collada object: unsupported number of scenes\n");
    }

    const VisualScene* scene = library.begin()->second;
    for (std::map<std::string, Node*>::const_iterator it = scene->nodes.begin(); status && (it != scene->nodes.end()); it++)
    {
        status = process_node(it->second, mesh_data);
    }

    return status;
}

bool Collada::Importer::process_image_library(const Parser::ImageLibrary& library, MeshData& mesh_data)
{
    bool status = true;

    for (Parser::ImageLibrary::const_iterator it = library.begin(); it != library.end(); it++)
    {
        const std::string& id = it->first;
        std::string texture = Filesystem::Path(*it->second->init_from.file_name).filename;
        if (id == "Colour") {
            mesh_data.colour_texture = texture;
        } else if (id == "Normal") {
            mesh_data.normal_texture = texture;
        } else if (id == "Height") {
            mesh_data.height_texture = texture;
        }
    }

    return status;
}

bool Collada::Importer::process_node(const Collada::Node* node, MeshData& mesh_data)
{
    bool status = true;

    MeshData::Node& n = *mesh_data.nodes.insert(mesh_data.nodes.end(), MeshData::Node());
    n.name = *node->name;
    n.parent = node->parent == nullptr ? "" : *node->parent->name;

    if (node->transform_type == Collada::Node::TRANSFORM_MATRIX)
    {
        n.offset_matrix = Matrix::Transpose(Matrix4F(node->transform.matrix));
    }
    else
    {
        Vector3F scale = Vector3F(node->transform.scale[0], node->transform.scale[1], node->transform.scale[2]);
        Vector3F rotation = Vector3F(node->transform.rotation_x[3] * M_PI / 180.0f, node->transform.rotation_y[3] * M_PI / 180.0f, node->transform.rotation_z[3] * M_PI / 180.0f);
        Vector3F translation = Vector3F(node->transform.translation[0], node->transform.translation[1], node->transform.translation[2]);

        // we are working with right handed matrices here - so the order is: scale -> rotate -> translate
        n.offset_matrix = Matrix4F::Scale(scale) * Quaternion(rotation).matrix() * Matrix4F::Translate(translation);
    }

    //printf("Node %s:\n", n.name.c_str());
    //n.offset_matrix.print();
    //printf("\n");

    if (node->extra.technique.roll != 0)
    {
        printf("warning: bone node \"%s\" has a non-zero roll value of %f\n", node->name->c_str(), node->extra.technique.roll);
    }

    m_node_map[n.name] = static_cast<unsigned int>(m_node_map.size());

    return status;
}

bool Collada::Importer::process_geometry(const Geometry* obj)
{
    bool status = true;

    IMesh& data = m_mesh_map.insert(std::pair<std::string, IMesh>(*obj->id, IMesh())).first->second;

    const Mesh& mesh = obj->mesh;

    const SourceArray* vertex_source = nullptr;
    const SourceArray* normal_source = nullptr;
    const SourceArray* uv_source = nullptr;

    unsigned int vertex_offset = 0;
    unsigned int normal_offset = 0;
    unsigned int uv_offset = 0;

    // TODO: this part assumes all the triangle arrays use the same sources - if thsi changes we will need to update this
    const TriangleArray& triangle_array_0 = *mesh.triangle_arrays;
    for (unsigned int i = 0; status && (i < triangle_array_0.num_inputs); i++)
    {
        const Input& input = triangle_array_0.inputs[i];

        std::string source_name = input.source->substr(1);
        const std::map<std::string, Source*>::const_iterator it = mesh.sources.find(source_name);
        if (it != mesh.sources.end())
        {
            switch (input.semantic)
            {
            case Input::VERTEX: { vertex_source = &it->second->array; vertex_offset = input.offset; break; }
            case Input::NORMAL: { normal_source = &it->second->array; normal_offset = input.offset; break; }
            case Input::TEXCOORD: { uv_source = &it->second->array; uv_offset = input.offset; break; }
            case Input::COLOR: { break; }
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
            printf("error: source \"%s\" not found\n", source_name.c_str());
        }
    }

    for (unsigned int i = 0; i < vertex_source->count; i += 3)
    {
        const float* array = vertex_source->float_array;

        IVertex vertex = { 0 };
        vertex.position = Vector3F(array[i + 0], array[i + 1], array[i + 2]);
        data.vertex_array.push_back(vertex);
    }

    for (unsigned int i = 0; i < normal_source->count; i += 3)
    {
        const float* array = normal_source->float_array;
        data.normal_array.push_back(Vector3F(array[i + 0], array[i + 1], array[i + 2]));
    }

    for (unsigned int i = 0; i < uv_source->count; i += 2)
    {
        const float* array = uv_source->float_array;
        data.uv_array.push_back(Vector2F(array[i + 0], array[i + 1]));
    }

    data.node_index = m_node_map[*obj->name];

    for (unsigned int i = 0; i < mesh.num_triangle_arrays; i++)
    {
        const TriangleArray& triangle_array = mesh.triangle_arrays[i];

        unsigned int num_vertices = triangle_array.count * triangle_array.num_inputs * 3;
        for (unsigned int j = 0; j < num_vertices; j += triangle_array.num_inputs)
        {
            data.indices.push_back(triangle_array.indices[j + vertex_offset]);
            data.indices.push_back(triangle_array.indices[j + normal_offset]);
            data.indices.push_back(triangle_array.indices[j + uv_offset]);
        }
    }

    return status;
}

bool Collada::Importer::process_mesh_data(MeshData& data)
{
    bool status = true;

    MeshData::VertexMap vertex_map;

    data.index_count = 0;

    for (std::map<std::string, IMesh>::const_iterator it = m_mesh_map.begin(); it != m_mesh_map.end(); it++)
    {
        const IMesh& mesh = it->second;
        
        data.meshes.push_back(MeshData::Mesh());
        MeshData::Mesh& mesh_data = data.meshes.back();
        mesh_data.name = it->first;

        for (unsigned int i = 0; i < mesh.indices.size(); i += 3)
        {
            const IVertex& v = mesh.vertex_array[mesh.indices[i + 0]];
            const Vector3F& n = mesh.normal_array[mesh.indices[i + 1]];
            const Vector2F& uv = mesh.uv_array[mesh.indices[i + 2]];

            MeshData::Vertex vertex = { 0 };
            vertex.position = v.position;
            vertex.normal = n;
            vertex.uv = uv;
            vertex.node_index = static_cast<unsigned char>(mesh.node_index);
            vertex.bone_count = v.bone_count;
            for (unsigned int i = 0; i < 4; i++) {
                vertex.bone_indices[i] = v.bone_indices[i];
                vertex.bone_weights[i] = v.bone_weights[i];
            }

            unsigned int vertex_index = 0;

            MeshData::VertexMap::const_iterator it = vertex_map.find(vertex);
            if (it == vertex_map.end())
            {
                vertex_index = (unsigned int) data.vertices.size();
                vertex_map[vertex] = vertex_index;
                data.vertices.push_back(vertex);
            }
            else
            {
                vertex_index = it->second;
            }

            mesh_data.indices.push_back(vertex_index);
        }

        data.index_count += mesh.indices.size() / 3;
    }

    return status;
}
