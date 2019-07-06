#include <gk/static_mesh.hpp>

#include <assert.h>
#include <string.h>

#include <map>

#include <gk/tga_image.hpp>

StaticMesh::StaticMesh() :
    Mesh(),
    RootNode("Root")
{
    m_DiffuseTexture = nullptr;
}

StaticMesh::StaticMesh(const MeshData& data, const std::string& texture_directory) :
	RootNode("Root")
{
	assert(data.Nodes.size() <= NODE_LIMIT);
	assert(data.Textures.size() != 0);

	std::map<std::string, unsigned int> node_map;

	// initial nodes
	Nodes.reserve(NODE_LIMIT);
	for (unsigned int i = 0; i < data.Nodes.size(); i++)
	{
		const MeshData::Node& node = data.Nodes[i];
		Nodes.push_back(Node(node.name, node.offset_matrix));
		node_map[node.name] = i;
	}

	StaticMesh::Vertex* vertex_buffer = new Vertex[data.VertexCount];
	StaticMesh::Vertex* vertex_iterator = vertex_buffer;
	for (unsigned int m = 0; m < data.Meshes.size(); m++)
	{
		const MeshData::Mesh& mesh = data.Meshes[m];
		const MeshData::Vertex* vertices = mesh.vertices.data();
		unsigned char node = static_cast<unsigned char>(node_map.at(mesh.name));

		for (unsigned int i = 0; i < mesh.vertices.size(); i++)
		{
			*vertex_iterator = StaticMesh::Vertex
			{
				{vertices[i].position.x, vertices[i].position.y, vertices[i].position.z},
				{vertices[i].normal.x, vertices[i].normal.y, vertices[i].normal.z},
				{vertices[i].uv.x, vertices[i].uv.y},
				node // or you can use MeshData::Vertex.node
			};
			vertex_iterator ++;
		}
	}

	unsigned short* index_buffer = new unsigned short[data.IndexCount];
	unsigned short* index_iterator = index_buffer;
	for(unsigned int m = 0; m < data.Meshes.size(); m++)
	{
		const MeshData::Mesh& mesh = data.Meshes[m];
		memcpy(index_iterator, &mesh.indices[0], sizeof(unsigned short) * mesh.indices.size());
		index_iterator += mesh.indices.size();
	}

	const RenderingContext* context = RenderingContext::GetInstance();

	m_VAO = new VertexArray();
	m_VAO->Bind();

	m_VBO = new VertexBuffer(GFX_ARRAY_BUFFER);
	m_VBO->Allocate(sizeof(StaticMesh::Vertex) * data.VertexCount, vertex_buffer, GFX_STATIC_DRAW);

	m_VAO->EnableVertexAttribute(VertexAttributes::VERTEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::NORMAL);
	m_VAO->EnableVertexAttribute(VertexAttributes::UV);
	m_VAO->EnableVertexAttribute(VertexAttributes::NODE);
	context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 3, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(StaticMesh::Vertex), (void*) offsetof(StaticMesh::Vertex, position));
	context->SetVertexAttributeLayoutF(VertexAttributes::NORMAL, 3, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(StaticMesh::Vertex), (void*) offsetof(StaticMesh::Vertex, normal));
	context->SetVertexAttributeLayoutF(VertexAttributes::UV,	 2, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(StaticMesh::Vertex), (void*) offsetof(StaticMesh::Vertex, uv));
	context->SetVertexAttributeLayoutI(VertexAttributes::NODE,	 1, GFX_TYPE_UNSIGNED_BYTE, sizeof(StaticMesh::Vertex), (void*) offsetof(StaticMesh::Vertex, node));
	
	m_IBO = new VertexBuffer(GFX_ELEMENT_BUFFER);
	m_IBO->Allocate(sizeof(unsigned short) * data.IndexCount, index_buffer, GFX_STATIC_DRAW);

	m_ElementCount = data.IndexCount;

	delete[] index_buffer;
	delete[] vertex_buffer;

	// process parent hierchy
	for (unsigned int i = 0; i < data.Nodes.size(); i++)
	{
		const MeshData::Node& node = data.Nodes[i];

		if (node.parent == "None") {
			Nodes[i].SetParent(&RootNode);
			//Nodes[i].SetParent(nullptr);
		} else {
			Nodes[i].SetParent(&Nodes[node_map.at(node.parent)]);
		}
	}

	TgaImage image((texture_directory + data.Textures[0]).c_str());
	m_DiffuseTexture = new Texture(image.HasAlpha() ? GFX_RGBA : GFX_RGB, image.GetWidth(), image.GetHeight(), GFX_TYPE_UNSIGNED_BYTE, image.GetPixels(), GFX_LINEAR, GFX_CLAMP_TO_EDGE);
}

StaticMesh::~StaticMesh()
{	
}

void StaticMesh::Destroy()
{
    if(m_DiffuseTexture != nullptr) {
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
