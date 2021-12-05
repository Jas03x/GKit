#ifndef GK_DYNAMIC_MESH_H
#define GK_DYNAMIC_MESH_H

#include <string>

#include <gk/3d/config.hpp>
#include <gk/3d/bone.hpp>
#include <gk/3d/node.hpp>
#include <gk/core/graphics/mesh.hpp>
#include <gk/core/graphics/texture.hpp>
#include <gk/core/importer/mesh_data.hpp>
#include <gk/core/math/transform_3d.hpp>

class DynamicMesh : public Mesh
{
public:
    enum
    {
        BONE_LIMIT = DYNAMIC_MESH_BONE_LIMIT,
        NODE_LIMIT = DYNAMIC_MESH_NODE_LIMIT
    };

    struct Vertex
    {
        float         position[3];
        float         normal[3];
        float         uv[2];
        unsigned char node;
        unsigned char bone_indices[4];
        float         bone_weights[4];
    };

    enum VertexAttributes
    {
        VERTEX      = 0,
        NORMAL      = 1,
        UV          = 2,
        NODE        = 3,
        BONE_INDEX  = 4,
        BONE_WEIGHT = 5
    };

protected:
    Texture* m_DiffuseTexture;

public:
    Node RootNode;
    std::vector<Bone> Bones;
    std::vector<Node> Nodes;

protected:
    void Load(const MeshData& data);

public:
    DynamicMesh();
    DynamicMesh(const MeshData& data);
    ~DynamicMesh();

    void Destroy();
    
    Node*          GetNode(const std::string& name);
    const Texture* GetDiffuseTexture() const;
};

#endif // GK_DYNAMIC_MESH_H
