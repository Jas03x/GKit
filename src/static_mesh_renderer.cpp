#include <gk/static_mesh_renderer.hpp>

#include <gk/config.hpp>
#include <gk/sun.hpp>
#include <gk/file.hpp>
#include <gk/camera_3d.hpp>

#include <assert.h>

StaticMeshRenderer* StaticMeshRenderer::Instance = nullptr;

StaticMeshRenderer::StaticMeshRenderer()
{
	Shader::Load(
		ShaderSource::FromFile(SHADER_BASE "static_mesh_renderer.vert"),
		ShaderSource::FromFile(SHADER_BASE "static_mesh_renderer.frag"),
		[](GFX_HANDLE id)
		{
			const RenderingContext* context = RenderingContext::GetInstance();

			context->SetAttributeLocation(id, StaticMesh::VertexAttributes::VERTEX, "vertex");
			context->SetAttributeLocation(id, StaticMesh::VertexAttributes::NORMAL, "normal");
			context->SetAttributeLocation(id, StaticMesh::VertexAttributes::UV, "uv");
			context->SetAttributeLocation(id, StaticMesh::VertexAttributes::NODE, "node");

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

StaticMeshRenderer::~StaticMeshRenderer()
{

}

void StaticMeshRenderer::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new StaticMeshRenderer();
	}
}

void StaticMeshRenderer::DeleteInstance()
{
	assert(Instance != nullptr);
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void StaticMeshRenderer::Bind()
{
	assert(Instance != nullptr);
	Instance->Shader::Bind();
}

void StaticMeshRenderer::Render(const StaticMesh& mesh)
{
	assert(Instance != nullptr);

	const RenderingContext* context = RenderingContext::GetInstance();
	mesh.Bind();

	Matrix4F v_matrix = Camera3D::GetInstance()->GetViewMatrix();

	Matrix4F vertex_matrices[StaticMesh::NODE_LIMIT];
	Matrix4F normal_matrices[StaticMesh::NODE_LIMIT];
	
	// the root node's offset is applied before the transform
	Matrix4F root_transform = mesh.RootNode.GetLocalTransform() * mesh.RootNode.GetOffsetMatrix();
	for (unsigned int i = 0; i < mesh.Nodes.size(); i++)
	{
		const Node& node = mesh.Nodes[i];
		int parent = node.GetParentIndex();

		const Matrix4F& parent_transform = (parent == -1) ? root_transform : vertex_matrices[parent];
		vertex_matrices[i] = parent_transform * node.GetOffsetMatrix() * node.GetLocalTransform();
	}

	for (unsigned int i = 0; i < mesh.Nodes.size(); i++)
	{
		vertex_matrices[i] = v_matrix * vertex_matrices[i];
		normal_matrices[i] = Matrix::Inverse(Matrix::Transpose(vertex_matrices[i]));
	}

	Vector3F sun_position = (v_matrix * Vector4F(Sun::Position, 1.0f)).xyz();

	context->LoadConstantMatrix4F(Instance->m_VertexMatricies, mesh.Nodes.size(), GFX_FALSE, &vertex_matrices[0][0][0]);
	context->LoadConstantMatrix4F(Instance->m_NormalMatricies, mesh.Nodes.size(), GFX_FALSE, &normal_matrices[0][0][0]);
	context->LoadConstantMatrix4F(Instance->m_ProjectionMatrix, 1, GFX_FALSE, &Camera3D::GetInstance()->GetProjectionMatrix()[0][0]);
	context->LoadConstantArray3F(Instance->m_SunPosition, 1, &sun_position[0]);
	context->LoadConstantArray3F(Instance->m_SunColor, 1, &Sun::Color[0]);

	mesh.GetDiffuseTexture()->Bind(Instance->m_DiffuseTexture, 0);

	//glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
	context->DrawElements(GFX_TRIANGLES, mesh.GetElementCount(), GFX_TYPE_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
}

void StaticMeshRenderer::Render(const StaticMesh& mesh, const std::vector<Transform3D>& transforms, bool useParentTransform)
{
	Render(mesh, transforms.data(), transforms.size(), useParentTransform);
}

void StaticMeshRenderer::Render(const StaticMesh& mesh, const Transform3D* transforms, unsigned int count, bool useParentTransform)
{
	assert(Instance != nullptr);

	const RenderingContext* context = RenderingContext::GetInstance();
	mesh.Bind();

	Matrix4F vertex_matrices[StaticMesh::NODE_LIMIT];
	Matrix4F normal_matrices[StaticMesh::NODE_LIMIT];
	Matrix4F v_matrix = Camera3D::GetInstance()->GetViewMatrix();
	// TODO: UPDATE FOR NEW NODE SYSTEM: Matrix4F parentInverse = useParentTransform ? Matrix4F() : Matrix::Inverse(mesh.RootNode.GetLocalMatrix());

	Vector3F sun_position = (v_matrix * Vector4F(Sun::Position, 1.0f)).xyz();

	context->LoadConstantArray3F(Instance->m_SunPosition, 1, &sun_position[0]);
	context->LoadConstantArray3F(Instance->m_SunColor, 1, &Sun::Color[0]);
	context->LoadConstantMatrix4F(Instance->m_ProjectionMatrix, 1, GFX_FALSE, &Camera3D::GetInstance()->GetProjectionMatrix()[0][0]);

	mesh.GetDiffuseTexture()->Bind(Instance->m_DiffuseTexture, 0);

	for (unsigned int m = 0; m < count; m++)
	{
		for (unsigned int i = 0; i < mesh.Nodes.size(); i++)
		{
			// TODO: UPDATE FOR NEW NODE SYSTEM: vertex_matrices[i] = v_matrix * transforms[m].ToMatrix() * parentInverse * mesh.Nodes[i].GetGlobalMatrix();
			normal_matrices[i] = Matrix::Inverse(Matrix::Transpose(vertex_matrices[i]));
		}

		context->LoadConstantMatrix4F(Instance->m_VertexMatricies, mesh.Nodes.size(), GFX_FALSE, &vertex_matrices[0][0][0]);
		context->LoadConstantMatrix4F(Instance->m_NormalMatricies, mesh.Nodes.size(), GFX_FALSE, &normal_matrices[0][0][0]);

		//glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
		context->DrawElements(GFX_TRIANGLES, mesh.GetElementCount(), GFX_TYPE_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
	}
}
