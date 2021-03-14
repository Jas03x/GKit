#ifndef MESH_DATA_HPP
#define MESH_DATA_HPP

#include <map>
#include <string>
#include <vector>

#include <gk/core/math/linear.hpp>

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

    enum Orientation
    {
        Y_UP = 0,
        Z_UP = 1,
        UNKNOWN = 2
    };

    std::string colour_texture;
    std::string normal_texture;
    std::string height_texture;

    std::vector<Bone> bones;
    std::vector<Node> nodes;
    std::vector<Vertex> vertices;

    Orientation orientation;

    unsigned int index_count;
    std::vector<Mesh> meshes;

    // vertex comparator function:
    struct VertexComparator
    {
        bool operator()(const MeshData::Vertex& v0, const MeshData::Vertex& v1) const;
    };
    typedef std::map<Vertex, unsigned short, VertexComparator> VertexMap;
};

#endif // MESH_DATA_HPP
