#ifndef MESH_DATA_HPP
#define MESH_DATA_HPP

#include <vector>

#include <gk/matrix.hpp>

struct MeshData
{
    struct Node
    {
        std::string name;
        std::string parent;

        Vector3F scale;
        Vector3F rotation;
        Vector3F translation;
    };

    std::vector<Node> nodes;
};

#endif // MESH_DATA_HPP