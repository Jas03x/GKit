#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include <vector>

#include <gk/3d/config/config.hpp>
#include <gk/3d/core/node.hpp>
#include <gk/importer/mesh_data.hpp>
#include <gk/graphics/mesh.hpp>
#include <gk/graphics/texture.hpp>
#include <gk/math/transform_3d.hpp>

class StaticMesh : public Mesh
{
public:
	enum
	{
		NODE_LIMIT = STATIC_MESH_NODE_LIMIT
	};

	typedef struct Vertex
	{
		float position[3];
		float normal[3];
		float uv[2];
		unsigned char node;
	} Vertex;

	typedef enum
	{
		VERTEX	= 0,
		NORMAL	= 1,
		UV		= 2,
		NODE	= 3
	} VertexAttributes;

protected:
	Texture* m_DiffuseTexture;

public:
	Node RootNode;
	std::vector<Node> Nodes;

protected:
	void Load(const MeshData& data, const std::string& texture_directory);

public:
    StaticMesh();
	StaticMesh(const MeshData& data, const std::string& texture_directory);
	~StaticMesh();
    
    void Destroy();

	Node*           GetNode(const std::string& name);
	const Texture*  GetDiffuseTexture() const;
};

#endif // STATIC_MESH_H
