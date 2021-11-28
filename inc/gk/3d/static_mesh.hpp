#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include <vector>

#include <gk/3d/config.hpp>
#include <gk/3d/node.hpp>
#include <gk/core/importer/mesh_data.hpp>
#include <gk/core/graphics/mesh.hpp>
#include <gk/core/graphics/texture.hpp>
#include <gk/core/math/transform_3d.hpp>

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
	Texture* m_AmbientTexture;
	Texture* m_DiffuseTexture;
	Texture* m_SpecularTexture;

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

	Node*          GetNode(const std::string& name);
	const Texture* GetAmbientTexture() const;
	const Texture* GetDiffuseTexture() const;
	const Texture* GetSpecularTexture() const;
};

#endif // STATIC_MESH_H
