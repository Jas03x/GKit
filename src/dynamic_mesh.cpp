#include <gk/dynamic_mesh.hpp>

#include <assert.h>

#include <map>

#include <gk/tga_image.hpp>

DynamicMesh::DynamicMesh() :
    Mesh(),
    RootNode("Root")
{
    m_DiffuseTexture = nullptr;
}

DynamicMesh::DynamicMesh(const MeshData& data, const std::string& texture_directory) :
    Mesh(),
    RootNode("Root")
{
	assert((data.bones.size() <= BONE_LIMIT) && (data.nodes.size() <= NODE_LIMIT));

	std::map<std::string, unsigned int> node_map;

	// initial nodes
	Nodes.reserve(NODE_LIMIT);
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		const MeshData::Node& node_data = data.nodes[i];
		Matrix4F offset_matrix = Matrix4F::Translate(node_data.translation) * Quaternion(node_data.rotation).matrix() * Matrix4F::Scale(node_data.scale);

		Nodes.push_back(Node(node_data.name, offset_matrix));
		node_map[node_data.name] = i;
	}

	// process parent hierchy
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		const MeshData::Node& node = data.nodes[i];
		
		if (node.parent.size() == 0) {
			Nodes[i].SetParent(&RootNode);
		} else {
			Nodes[i].SetParent(&Nodes[node_map.at(node.parent)]);
		}
	}

	Bones.reserve(BONE_LIMIT);
	for(unsigned int i = 0; i < data.bones.size(); i++)
	{
		const MeshData::Bone& bone = data.bones[i];
		Bones.push_back(Bone(bone.name, &Nodes[node_map.at(bone.name)], bone.offset_matrix));
	}

	DynamicMesh::Vertex* vertex_buffer = new DynamicMesh::Vertex[data.vertices.size()];
	for (unsigned int i = 0; i < data.vertices.size(); i++)
	{
		const MeshData::Vertex& v = data.vertices[i];

		vertex_buffer[i] = DynamicMesh::Vertex
		{
			{ v.position.x, v.position.y, v.position.z },
			{ v.normal.x,   v.normal.y,   v.normal.z },
			{ v.uv.x,       v.uv.y },
			v.node,
			{ v.bone_indices[0], v.bone_indices[1], v.bone_indices[2], v.bone_indices[3] },
			{ v.bone_weights[0], v.bone_weights[1], v.bone_weights[2], v.bone_weights[3] }
		};

		/*
		printf("Vertex: (%f, %f, %f), (%hhu, %hhu, %hhu, %hhu), (%f, %f, %f, %f)\n",
			v.position.x, v.position.y, v.position.z,
			v.bone_indices[0], v.bone_indices[1], v.bone_indices[2], v.bone_indices[3],
			v.bone_weights[0], v.bone_weights[1], v.bone_weights[2], v.bone_weights[3]
		);
		*/
		
		//float sum = vertices[i].bones[0].weight + vertices[i].bones[1].weight + vertices[i].bones[2].weight + vertices[i].bones[3].weight;
		//GK_ASSERT((sum >= 0.99f) && (sum <= 1.01f), ("Error: Vertex weights do not add to one: %f + %f + %f + %f = %f\n", vertices[i].bones[0].weight, vertices[i].bones[1].weight, vertices[i].bones[2].weight, vertices[i].bones[3].weight, sum));
	}

	const RenderingContext* context = RenderingContext::GetInstance();

	m_VAO = new VertexArray();
	m_VAO->Bind();

	m_VBO = new VertexBuffer(GFX_ARRAY_BUFFER);
	m_VBO->Allocate(sizeof(DynamicMesh::Vertex) * data.vertices.size(), vertex_buffer, GFX_STATIC_DRAW);

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
	m_IBO->Allocate(sizeof(unsigned short) * data.indices.size(), data.indices.data(), GFX_STATIC_DRAW);

	m_ElementCount = data.indices.size();

	delete[] vertex_buffer;

	TgaImage image((texture_directory + data.diffuse_texture).c_str());
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
