#ifndef COLLADA_IMPORTER_HPP
#define COLLADA_IMPORTER_HPP

#include <gk/mesh_data.hpp>

namespace Collada
{
    class Importer
    {
    private:
        Importer();
        ~Importer();

        bool process_geometry(const Parser::GeometryLibrary& library, MeshData& data);
        bool process_controllers(const Parser::ControllerLibrary& library, MeshData& data);
        bool process_scene(const Parser::SceneLibrary& library, MeshData& data);

        bool process_node(const Node* node, MeshData& data);

    public:
        static bool Import(const char* path, MeshData& data);
    };
}

#endif // COLLADA_IMPORTER_HPP