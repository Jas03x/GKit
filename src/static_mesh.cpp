#include <gk/static_mesh.hpp>

#include <assert.h>
#include <string.h>

#include <map>

#include <gk/tga_image.hpp>

StaticMesh::StaticMesh() :
	Mesh(),
	RootNode("Root", Matrix4F(1.0f))
{
	m_DiffuseTexture = nullptr;
}

StaticMesh::StaticMesh(const MeshData& data, const std::string& texture_directory) :
	RootNode("Root", Matrix4F(1.0f))
{
	assert(data.nodes.size() <= NODE_LIMIT);

	std::map<std::string, std::vector<unsigned int>> node_hierarchy;

	// initial nodes
	Nodes.reserve(NODE_LIMIT);
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		const MeshData::Node& node = data.nodes[i];
		Nodes.push_back(Node(node.name, node.offset_matrix));

		if (node.parent.size() > 0)
		{
			node_hierarchy[node.parent].push_back(i);
		}
	}

	// process parent hierchy
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		Node& node = Nodes[i];
		std::vector<unsigned int> children = node_hierarchy[node.GetName()];

		if (children.size() > 0)
		{
			node.SetChildren(children.size(), children.data());
		}
	}

	StaticMesh::Vertex* vertex_buffer = new Vertex[data.vertices.size()];
	StaticMesh::Vertex* vertex_iterator = vertex_buffer;

	for (unsigned int i = 0; i < data.vertices.size(); i++)
	{
		const MeshData::Vertex& v = data.vertices[i];

		*vertex_iterator = StaticMesh::Vertex
		{
			{ v.position.x, v.position.y, v.position.z },
			{ v.normal.x, v.normal.y, v.normal.z },
			{ v.uv.x, v.uv.y },
			v.node
		};
		vertex_iterator++;
	}

	const RenderingContext* context = RenderingContext::GetInstance();

	m_VAO = new VertexArray();
	m_VAO->Bind();

	m_VBO = new VertexBuffer(GFX_ARRAY_BUFFER);
	m_VBO->Allocate(sizeof(StaticMesh::Vertex) * data.vertices.size(), vertex_buffer, GFX_STATIC_DRAW);

	m_VAO->EnableVertexAttribute(VertexAttributes::VERTEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::NORMAL);
	m_VAO->EnableVertexAttribute(VertexAttributes::UV);
	m_VAO->EnableVertexAttribute(VertexAttributes::NODE);
	context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, position));
	context->SetVertexAttributeLayoutF(VertexAttributes::NORMAL, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, normal));
	context->SetVertexAttributeLayoutF(VertexAttributes::UV, 2, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, uv));
	context->SetVertexAttributeLayoutI(VertexAttributes::NODE, 1, GFX_TYPE_UNSIGNED_BYTE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, node));

	m_IBO = new VertexBuffer(GFX_ELEMENT_BUFFER);
	m_IBO->Allocate(sizeof(unsigned short) * data.indices.size(), data.indices.data(), GFX_STATIC_DRAW);

	m_ElementCount = data.indices.size();

	delete[] vertex_buffer;

	TgaImage image((texture_directory + data.diffuse_texture).c_str());
	m_DiffuseTexture = new Texture(image.HasAlpha() ? GFX_RGBA : GFX_RGB, image.GetWidth(), image.GetHeight(), GFX_TYPE_UNSIGNED_BYTE, image.GetPixels(), GFX_LINEAR, GFX_CLAMP_TO_EDGE);
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::Destroy()
{
	if (m_DiffuseTexture != nullptr) {
		delete m_DiffuseTexture;
		m_DiffuseTexture = nullptr;
	}

	Mesh::Destroy();
}

Node* StaticMesh::GetNode(const std::string& name)
{
	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (Nodes[i].GetName() == name)
		{
			return &Nodes[i];
		}
	}

	return nullptr;
}

const Texture* StaticMesh::GetDiffuseTexture() const
{
	return m_DiffuseTexture;
}
