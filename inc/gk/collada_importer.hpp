#ifndef COLLADA_IMPORTER_HPP
#define COLLADA_IMPORTER_HPP

#include <gk/mesh_data.hpp>

namespace Collada
{
    class Importer
    {
    private:
        std::map<std::string, unsigned int> m_node_map;

    private:
        Importer();
        ~Importer();

        bool process_geometry_library(const Parser::GeometryLibrary& library, MeshData& data);
        bool process_controller_library(const Parser::ControllerLibrary& library, MeshData& data);
        bool process_scene_library(const Parser::SceneLibrary& library, MeshData& data);

        bool process_node(const Node* node, MeshData& data);
        bool process_geometry(const Geometry* obj, MeshData& data);

    public:
        static bool Import(const char* path, MeshData& data);
    };
}

#endif // COLLADA_IMPORTER_HPP