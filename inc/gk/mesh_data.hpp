#ifndef MESH_DATA_HPP
#define MESH_DATA_HPP

#include <string>
#include <vector>

#include <gk/matrix.hpp>
#include <gk/mesh_orientation.hpp>

struct MeshData
{
    struct Node
    {
        std::string name;
        std::string parent;

        Matrix4F offset_matrix;
    };

    struct Vertex
    {
        Vector3F position;
        Vector3F normal;
        Vector2F uv;
        unsigned char node_index;
        unsigned char bone_indices[4];
        float bone_weights[4];
        unsigned int bone_count;
    };

    struct Bone
    {
        std::string name;
        Matrix4F bind_pose_matrix;
    };

    struct Mesh
    {
        std::string name;
        std::vector<unsigned short> indices;
    };

    std::string diffuse_texture;

    std::vector<Bone> bones;
    std::vector<Node> nodes;
    std::vector<Vertex> vertices;

    Orientation orientation;

    unsigned int index_count;
    std::vector<Mesh> meshes;
};

#endif // MESH_DATA_HPP