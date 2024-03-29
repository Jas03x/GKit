#include <gk/3d/dynamic_mesh.hpp>

#include <cassert>

#include <map>
#include <utility>

#include <gk/3d/texture_manager.hpp>

DynamicMesh::DynamicMesh() : RootNode("Root", Matrix4F(1.0f))
{
	m_DiffuseTexture = nullptr;
}

DynamicMesh::DynamicMesh(const MeshData& data) : RootNode("Root", Matrix4F(1.0f))
{
	this->Load(data);
}

void DynamicMesh::Load(const MeshData& data)
{
	assert((data.bones.size() <= BONE_LIMIT) && (data.nodes.size() <= NODE_LIMIT));

	std::map<std::string, unsigned int> node_map;

	// initialize nodes
	Nodes.reserve(NODE_LIMIT);
	for (unsigned int i = 0; i < data.nodes.size(); i++)
	{
		const MeshData::Node& node = data.nodes[i];
		Nodes.push_back(Node(node.name, node.offset_matrix));
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

	Bones.reserve(BONE_LIMIT);
	for (unsigned int i = 0; i < data.bones.size(); i++)
	{
		const MeshData::Bone& bone = data.bones[i];
		Bones.push_back(Bone(bone.name, node_map.at(bone.name), bone.bind_pose_matrix));
	}

	if (data.orientation == MeshData::Orientation::Z_UP)
	{
		this->RootNode.SetOffsetMatrix(Quaternion(-M_PI / 2.0f, 0.0f, 0.0f).matrix());
	}

	std::vector<DynamicMesh::Vertex> vertex_buffer(data.vertices.size());
	for (unsigned int i = 0; i < data.vertices.size(); i++)
	{
		const MeshData::Vertex& v = data.vertices[i];

		vertex_buffer[i] = DynamicMesh::Vertex
		{
			{ v.position.x, v.position.y, v.position.z },
			{ v.normal.x,   v.normal.y,   v.normal.z },
			{ v.uv.x,       v.uv.y },
			v.node_index,
			{ v.bone_indices[0], v.bone_indices[1], v.bone_indices[2], v.bone_indices[3] },
			{ v.bone_weights[0], v.bone_weights[1], v.bone_weights[2], v.bone_weights[3] }
		};

		float weight_sum = v.bone_weights[0] + v.bone_weights[1] + v.bone_weights[2] + v.bone_weights[3];
		if ((weight_sum < 0.99f) || (weight_sum > 1.01f))
		{
			printf("WARNING: vertex %u has invalid bone weight sum %f\n", i, weight_sum);
		}

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
	m_VBO->Bind();
	m_VBO->Allocate(sizeof(DynamicMesh::Vertex) * data.vertices.size(), vertex_buffer.data(), GFX_STATIC_DRAW);
	
	m_VAO->EnableVertexAttribute(VertexAttributes::VERTEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::NORMAL);
	m_VAO->EnableVertexAttribute(VertexAttributes::UV);
	m_VAO->EnableVertexAttribute(VertexAttributes::NODE);
	m_VAO->EnableVertexAttribute(VertexAttributes::BONE_INDEX);
	m_VAO->EnableVertexAttribute(VertexAttributes::BONE_WEIGHT);
	context->SetVertexAttributeLayoutF(VertexAttributes::VERTEX, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(DynamicMesh::Vertex), (void*)offsetof(DynamicMesh::Vertex, position));
	context->SetVertexAttributeLayoutF(VertexAttributes::NORMAL, 3, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(DynamicMesh::Vertex), (void*)offsetof(DynamicMesh::Vertex, normal));
	context->SetVertexAttributeLayoutF(VertexAttributes::UV, 2, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(DynamicMesh::Vertex), (void*)offsetof(DynamicMesh::Vertex, uv));
	context->SetVertexAttributeLayoutI(VertexAttributes::NODE, 1, GFX_TYPE_UNSIGNED_BYTE, sizeof(DynamicMesh::Vertex), (void*)offsetof(DynamicMesh::Vertex, node));
	context->SetVertexAttributeLayoutI(VertexAttributes::BONE_INDEX, 4, GFX_TYPE_UNSIGNED_BYTE, sizeof(DynamicMesh::Vertex), (void*)offsetof(DynamicMesh::Vertex, bone_indices));
	context->SetVertexAttributeLayoutF(VertexAttributes::BONE_WEIGHT, 4, GFX_TYPE_FLOAT, GFX_FALSE, sizeof(DynamicMesh::Vertex), (void*)offsetof(DynamicMesh::Vertex, bone_weights));

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

	m_DiffuseTexture = TextureManager::GetInstance()->Load(data.texture_directory + data.diffuse_texture);
}

DynamicMesh::~DynamicMesh()
{
}

void DynamicMesh::Destroy()
{
	Mesh::Destroy();
}

Node* DynamicMesh::GetNode(const std::string& name)
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

const Texture* DynamicMesh::GetDiffuseTexture() const
{
	return m_DiffuseTexture;
}
