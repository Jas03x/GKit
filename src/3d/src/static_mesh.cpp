#include <gk/3d/static_mesh.hpp>

#include <cassert>
#include <cstring>

#include <map>

#include <gk/3d/texture_manager.hpp>

StaticMesh::StaticMesh() : RootNode("Root", Matrix4F(1.0f))
{
	m_DiffuseTexture = nullptr;
}

StaticMesh::StaticMesh(const MeshData& data, const std::string& texture_directory) : RootNode("Root", Matrix4F(1.0f))
{
	this->Load(data, texture_directory);
}

void StaticMesh::Load(const MeshData& data, const std::string& texture_directory)
{
	assert(data.nodes.size() <= NODE_LIMIT);

	std::map<std::string, unsigned int> node_map;

	// initialize nodes
	Nodes.resize(data.nodes.size());
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		const MeshData::Node& node = data.nodes[i];
		Nodes[i] = Node(node.name, node.offset_matrix);
		node_map[node.name] = i;
	}

	// set node parents
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		const MeshData::Node& node_data = data.nodes[i];
		if(node_data.parent.size() > 0)
		{
			Nodes[i].SetParentIndex(node_map.at(node_data.parent));
		}
	}

	if (data.orientation == MeshData::Orientation::Z_UP)
	{
		this->RootNode.SetOffsetMatrix(Quaternion(-M_PI / 2.0f, 0.0f, 0.0f).matrix());
	}

	std::vector<StaticMesh::Vertex> vertex_buffer(data.vertices.size());
	for (unsigned int i = 0; i < data.vertices.size(); i++)
	{
		const MeshData::Vertex& v = data.vertices[i];

		vertex_buffer[i] = StaticMesh::Vertex
		{
			{ v.position.x, v.position.y, v.position.z },
			{ v.normal.x, v.normal.y, v.normal.z },
			{ v.uv.x, v.uv.y },
			v.node_index
		};
	}

	const RenderingContext* context = RenderingContext::GetInstance();

	m_VAO = new VertexArray();
	m_VAO->Bind();

	m_VBO = new VertexBuffer(GFX_ARRAY_BUFFER);
	m_VBO->Bind();
	m_VBO->Allocate(sizeof(StaticMesh::Vertex) * data.vertices.size(), vertex_buffer.data(), GFX_STATIC_DRAW);

	m_VAO->EnableVertexAttribute(VertexAttributes::VERTEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::NORMAL);
	m_VAO->EnableVertexAttribute(VertexAttributes::UV);
	m_VAO->EnableVertexAttribute(VertexAttributes::NODE);
	context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, position));
	context->SetVertexAttributeLayoutF(VertexAttributes::NORMAL, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, normal));
	context->SetVertexAttributeLayoutF(VertexAttributes::UV, 2, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, uv));
	context->SetVertexAttributeLayoutI(VertexAttributes::NODE, 1, GFX_TYPE_UNSIGNED_BYTE, sizeof(StaticMesh::Vertex), (void*)offsetof(StaticMesh::Vertex, node));

	m_IBO = new VertexBuffer(GFX_ELEMENT_BUFFER);
	m_IBO->Bind();
	m_IBO->Allocate(sizeof(unsigned short) * data.index_count, nullptr, GFX_STATIC_DRAW);

	size_t offset = 0;
	for (unsigned int i = 0; i < data.meshes.size(); i++)
	{
		const MeshData::Mesh& mesh = data.meshes[i];

		size_t size = mesh.indices.size() * sizeof(unsigned short);
		m_IBO->Update(offset, size, mesh.indices.data());

		offset += size;
	}

	m_ElementCount = data.index_count;

	m_DiffuseTexture = TextureManager::GetInstance()->Load(data.colour_texture);
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::Destroy()
{
	Mesh::Destroy();
}

Node* StaticMesh::GetNode(const std::string& name)
{
	Node* node = nullptr;

	for (unsigned int i = 0; i < Nodes.size(); i++)
	{
		if (Nodes[i].GetName() == name)
		{
			node = &Nodes[i];
			break;
		}
	}

	if (node == nullptr)
	{
		printf("WARNING: Could not find node %s\n", name.c_str());
	}

	return node;
}

const Texture* StaticMesh::GetDiffuseTexture() const
{
	return m_DiffuseTexture;
}
