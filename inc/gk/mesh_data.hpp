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

    struct Vertex
    {
        Vector3F position;
        Vector3F normal;
        Vector2F uv;
        unsigned char node;
        unsigned char bone_indices[4];
        float bone_weights[4];
        unsigned int bone_count;
    };

    struct Bone
    {
        std::string name;
        Matrix4F offset_matrix;
    };

    Matrix4F bind_pose_matrix;

    std::vector<Bone> bones;
    std::vector<Node> nodes;
    std::vector<Vertex> vertices;
};

#endif // MESH_DATA_HPP