#ifndef COLLADA_IMPORTER_HPP
#define COLLADA_IMPORTER_HPP

#include <map>
#include <string>
#include <vector>

#include <gk/core/math/linear.hpp>
#include <gk/core/importer/collada_parser.hpp>
#include <gk/core/importer/mesh_data.hpp>

namespace Collada
{
    class Importer
    {
    private:
        struct IVertex // intermediate data - vertex
        {
            Vector3F position;
            unsigned char bone_indices[4];
            float bone_weights[4];
            unsigned int bone_count;
        };

        struct IMesh // intermediate data - mesh
        {
            std::vector<IVertex>  vertex_array;
            std::vector<Vector3F> normal_array;
            std::vector<Vector2F> uv_array;
            
            unsigned int node_index;
            std::vector<unsigned short> indices;
        };

        std::map<std::string, IMesh> m_mesh_map;
        std::map<std::string, unsigned int> m_node_map;

    private:
        Importer();
        ~Importer();

        bool process_asset_info(const Collada::AssetInfo& info, MeshData& mesh_data);
        bool process_geometry_library(const Parser::GeometryLibrary& library);
        bool process_controller_library(const Parser::ControllerLibrary& library, MeshData& mesh_data);
        bool process_scene_library(const Parser::SceneLibrary& library, MeshData& mesh_data);
        bool process_image_library(const Parser::ImageLibrary& library, MeshData& mesh_data);

        bool process_node(const Node* node, MeshData& mesh_data);
        bool process_geometry(const Geometry* obj);
        bool process_mesh_data(MeshData& mesh_data);

    public:
        static bool Import(const char* path, MeshData& mesh_data);
    };
}

#endif // COLLADA_IMPORTER_HPP
