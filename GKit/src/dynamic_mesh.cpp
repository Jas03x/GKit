#include <gk/dynamic_mesh.hpp>

#include <map>

#include <gk/assert.hpp>
#include <gk/tga_image.hpp>

DynamicMesh::DynamicMesh() :
    Mesh(),
    RootNode("Root", Vector3F(0.0f), Quaternion(Vector3F(0.0f)), Vector3F(1.0f))
{
    m_DiffuseTexture = nullptr;
}

DynamicMesh::DynamicMesh(const MeshData& data, const std::string& texture_directory) :
    Mesh(),
    RootNode("Root", Vector3F(0.0f), Quaternion(Vector3F(0.0f)), Vector3F(1.0f))
{
	GK_ASSERT((data.Bones.size() <= BONE_LIMIT) && (data.Nodes.size() <= NODE_LIMIT), ("Error: Mode exceeds bone/node limit!\n"));
	GK_ASSERT(data.Textures.size() != 0, ("Error: Model has zero textures\n"));

	std::map<std::string, unsigned int> node_map;

	// initial nodes
	Nodes.reserve(NODE_LIMIT);
	for (unsigned int i = 0; i < data.Nodes.size(); i++)
	{
		const MeshData::Node& node = data.Nodes[i];
		Nodes.push_back(Node(node.name, node.position, Quaternion(node.rotation), node.scale));
		node_map[node.name] = i;
	}

	// process parent hierchy
	for (unsigned int i = 0; i < data.Nodes.size(); i++)
	{
		const MeshData::Node& node = data.Nodes[i];

		if (node.parent == "None") {
			//Nodes[i].SetParent(&RootNode);
			Nodes[i].SetParent(nullptr);
		} else {
			Nodes[i].SetParent(&Nodes[node_map.at(node.parent)]);
		}
	}

	Bones.reserve(BONE_LIMIT);
	for(unsigned int i = 0; i < data.Bones.size(); i++)
	{
		const MeshData::Bone& bone = data.Bones[i];
		Bones.push_back(Bone(bone.name, &Nodes[node_map.at(bone.name)], bone.matrix));
	}

	DynamicMesh::Vertex* vertex_buffer = new Vertex[data.VertexCount];
	DynamicMesh::Vertex* vertex_iterator = vertex_buffer;
	for (unsigned int m = 0; m < data.Meshes.size(); m++)
	{
		const MeshData::Mesh& mesh = data.Meshes[m];
		const MeshData::Vertex* vertices = mesh.vertices.data();
		unsigned char node = static_cast<unsigned char>(node_map.at(mesh.name));

		for (unsigned int i = 0; i < mesh.vertices.size(); i++)
		{
			*vertex_iterator = DynamicMesh::Vertex
			{
				{ vertices[i].position.x, vertices[i].position.y, vertices[i].position.z },
				{ vertices[i].normal.x,   vertices[i].normal.y,   vertices[i].normal.z },
				{ vertices[i].uv.x,       vertices[i].uv.y },
				node, // or you can use MeshData::Vertex.node
				{ vertices[i].bones[0].index,  vertices[i].bones[1].index,  vertices[i].bones[2].index,  vertices[i].bones[3].index  },
				{ vertices[i].bones[0].weight, vertices[i].bones[1].weight, vertices[i].bones[2].weight, vertices[i].bones[3].weight }
			};

			printf("Vertex: (%f, %f, %f), (%hhu, %hhu, %hhu, %hhu), (%f, %f, %f, %f)\n",
				vertex_iterator->position[0], vertex_iterator->position[1], vertex_iterator->position[2],
				vertex_iterator->bone_indices[0], vertex_iterator->bone_indices[1], vertex_iterator->bone_indices[2], vertex_iterator->bone_indices[3], 
				vertex_iterator->bone_weights[0], vertex_iterator->bone_weights[1], vertex_iterator->bone_weights[2], vertex_iterator->bone_weights[3]
			);
			
			//float sum = vertices[i].bones[0].weight + vertices[i].bones[1].weight + vertices[i].bones[2].weight + vertices[i].bones[3].weight;
			//GK_ASSERT((sum >= 0.99f) && (sum <= 1.01f), ("Error: Vertex weights do not add to one: %f + %f + %f + %f = %f\n", vertices[i].bones[0].weight, vertices[i].bones[1].weight, vertices[i].bones[2].weight, vertices[i].bones[3].weight, sum));
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

	RenderingContext* context = RenderingContext::GetInstance();

	m_VAO = new VertexArray();
	m_VAO->Bind();

	m_VBO = new VertexBuffer(GFX_ARRAY_BUFFER);
	m_VBO->Allocate(sizeof(DynamicMesh::Vertex) * data.VertexCount, vertex_buffer, GFX_STATIC_DRAW);

	m_VAO->EnableVertexAttribute(VertexAttributes::VERTEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::NORMAL);
	m_VAO->EnableVertexAttribute(VertexAttributes::UV);
	m_VAO->EnableVertexAttribute(VertexAttributes::NODE);
	m_VAO->EnableVertexAttribute(VertexAttributes::BONE_INDEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::BONE_WEIGHT);
	context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 	  3, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(DynamicMesh::Vertex), (void*) offsetof(DynamicMesh::Vertex, position));
	context->SetVertexAttributeLayoutF(VertexAttributes::NORMAL, 	  3, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(DynamicMesh::Vertex), (void*) offsetof(DynamicMesh::Vertex, normal));
	context->SetVertexAttributeLayoutF(VertexAttributes::UV,	  	  2, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(DynamicMesh::Vertex), (void*) offsetof(DynamicMesh::Vertex, uv));
	context->SetVertexAttributeLayoutI(VertexAttributes::NODE,	      1, GFX_TYPE_UNSIGNED_BYTE,	 sizeof(DynamicMesh::Vertex), (void*) offsetof(DynamicMesh::Vertex, node));
	context->SetVertexAttributeLayoutI(VertexAttributes::BONE_INDEX,  4, GFX_TYPE_UNSIGNED_BYTE,	 sizeof(DynamicMesh::Vertex), (void*) offsetof(DynamicMesh::Vertex, bone_indices));
	context->SetVertexAttributeLayoutF(VertexAttributes::BONE_WEIGHT, 4, GFX_TYPE_FLOAT, GFX_FALSE,  sizeof(DynamicMesh::Vertex), (void*) offsetof(DynamicMesh::Vertex, bone_weights));
	
	m_IBO = new VertexBuffer(GFX_ELEMENT_BUFFER);
	m_IBO->Allocate(sizeof(unsigned short) * data.IndexCount, index_buffer, GFX_STATIC_DRAW);

	m_ElementCount = data.IndexCount;

	delete[] index_buffer;
	delete[] vertex_buffer;

	TgaImage image((texture_directory + data.Textures[0]).c_str());
	m_DiffuseTexture = new Texture(image.HasAlpha() ? GFX_RGBA : GFX_RGB, image.GetWidth(), image.GetHeight(), GFX_TYPE_UNSIGNED_BYTE, image.GetPixels(), GFX_LINEAR, GFX_CLAMP_TO_EDGE);
}

DynamicMesh::~DynamicMesh()
{
}

void DynamicMesh::Destroy()
{
    if(m_DiffuseTexture != nullptr) {
		delete m_DiffuseTexture; 
		m_DiffuseTexture = nullptr;
	}

	Mesh::Destroy();
}

Node* DynamicMesh::GetNode(const std::string& name)
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

const Texture* DynamicMesh::GetDiffuseTexture() const
{
	return m_DiffuseTexture;
}
