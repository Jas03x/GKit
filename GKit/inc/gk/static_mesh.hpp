#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include <vector>

#include <gk/config.hpp>
#include <gk/mesh.hpp>
#include <gk/node.hpp>
#include <gk/mesh_data.hpp>
#include <gk/texture.hpp>
#include <gk/transform_3d.hpp>

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
	std::vector<Node> Nodes;
	Node RootNode;

public:
    StaticMesh();
	StaticMesh(const MeshData& data, const std::string& texture_directory);
	~StaticMesh();
    
    void Destroy();

	Node*           GetNode(const std::string& name);
	const Texture*  GetDiffuseTexture() const;
};

#endif // STATIC_MESH_H
