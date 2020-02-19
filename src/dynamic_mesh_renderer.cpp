#include <gk/dynamic_mesh_renderer.hpp>

#include <gk/sun.hpp>
#include <gk/file.hpp>
#include <gk/camera_3d.hpp>

#include <assert.h>

DynamicMeshRenderer* DynamicMeshRenderer::Instance = nullptr;

DynamicMeshRenderer::DynamicMeshRenderer()
{
	Shader::Load(
		ShaderSource::FromFile(SHADER_BASE "dynamic_mesh_renderer.vert"),
		ShaderSource::FromFile(SHADER_BASE "dynamic_mesh_renderer.frag"),
		[](GFX_HANDLE id)
		{
			const RenderingContext* context = RenderingContext::GetInstance();

			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::VERTEX, "vertex");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::NORMAL, "normal");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::UV, "uv");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::NODE, "node");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::BONE_INDEX, "bone_indices");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::BONE_WEIGHT, "bone_weights");

			context->SetFragmentLocation(id, 0, "fragment_color");
		}
	);

	m_SunColor = Shader::GetUniformLocation("sun_color");
	m_SunPosition = Shader::GetUniformLocation("sun_position");
	m_DiffuseTexture = Shader::GetUniformLocation("diffuse_texture");
	m_VertexMatricies = Shader::GetUniformLocation("vertex_matrices");
	m_NormalMatricies = Shader::GetUniformLocation("normal_matrices");
	m_ProjectionMatrix = Shader::GetUniformLocation("projection_matrix");
}

DynamicMeshRenderer::~DynamicMeshRenderer()
{

}

void DynamicMeshRenderer::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new DynamicMeshRenderer();
	}
}

void DynamicMeshRenderer::DeleteInstance()
{
	assert(Instance != nullptr);
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void DynamicMeshRenderer::Bind()
{
	assert(Instance != nullptr);
	Instance->Shader::Bind();
}

void CalculateSkinningMatrices(const Node* nodes, const Bone* bones, unsigned int index, Matrix4F* skinning_matrices, const Matrix4F& parent_transform)
{
	const Node& node = nodes[index];

	Matrix4F global_transform = parent_transform * node.GetOffsetMatrix() * node.GetLocalTransform();

	skinning_matrices[index] = global_transform;

	for (unsigned int i = 0; i < node.GetNumChildren(); i++)
	{
		printf("%s -> %s\n", node.GetName().c_str(), nodes[node.GetChildren()[i]].GetName().c_str());
		//printf("\nNode %s global transform:\n", node.GetName().c_str());
		//global_transform.print();
		CalculateSkinningMatrices(nodes, bones, node.GetChildren()[i], skinning_matrices, global_transform);
	}
}

void DynamicMeshRenderer::Render(const DynamicMesh& mesh)
{
	assert(Instance != nullptr);

	const RenderingContext* context = RenderingContext::GetInstance();
	mesh.Bind();

	Matrix4F v_matrix = Camera3D::GetInstance()->GetViewMatrix();

	Matrix4F skinning_matrices[DynamicMesh::BONE_LIMIT];

	Matrix4F vertex_matrices[DynamicMesh::BONE_LIMIT];
	Matrix4F normal_matrices[DynamicMesh::BONE_LIMIT];

	Matrix4F scene_matrix = Matrix4F(
		Vector4F(1, 0, 0, 0),
		Vector4F(0, 0, -1, 0),
		Vector4F(0, 1, 0, 0),
		Vector4F(0, 0, 0, 1)
	);

	CalculateSkinningMatrices(mesh.Nodes.data(), mesh.Bones.data(), 0, skinning_matrices, scene_matrix * mesh.RootNode.GetLocalTransform());

	for (unsigned int i = 0; i < mesh.Bones.size(); i++)
	{
		vertex_matrices[i] = skinning_matrices[mesh.Bones[i].GetNodeID()] * mesh.Bones[i].GetBindPoseMatrix();
		normal_matrices[i] = Matrix::Inverse(Matrix::Transpose(vertex_matrices[i]));

		printf("Bone %s skinning matrix:\n", mesh.Bones[i].GetName().c_str());
		vertex_matrices[i].print();
		printf("\n");
	}

	context->LoadConstantMatrix4F(Instance->m_VertexMatricies, mesh.Bones.size(), GFX_FALSE, &vertex_matrices[0][0][0]);
	context->LoadConstantMatrix4F(Instance->m_NormalMatricies, mesh.Bones.size(), GFX_FALSE, &normal_matrices[0][0][0]);

	Matrix4F mvp = Camera3D::GetInstance()->GetProjectionMatrix() * v_matrix;// *mesh.RootNode.GetLocalMatrix();
	context->LoadConstantMatrix4F(Instance->m_ProjectionMatrix, 1, GFX_FALSE, &mvp[0][0]);

	Vector3F sun_position = (v_matrix * Vector4F(Sun::Position, 1.0f)).xyz();
	context->LoadConstantArray3F(Instance->m_SunPosition, 1, &sun_position[0]);
	context->LoadConstantArray3F(Instance->m_SunColor, 1, &Sun::Color[0]);

	mesh.GetDiffuseTexture()->Bind(Instance->m_DiffuseTexture, 0);

	//glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
	context->DrawElements(GFX_TRIANGLES, mesh.GetElementCount(), GFX_TYPE_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
}
