#include <gk/3d/renderer/static_mesh_renderer.hpp>

#include <gk/3d/config.hpp>
#include <gk/3d/sun.hpp>
#include <gk/3d/camera_3d.hpp>
#include <gk/core/io/file.hpp>

#include <cassert>

#include "../../shdr/static_mesh_pixel_shader.hpp"
#include "../../shdr/static_mesh_vertex_shader.hpp"

StaticMeshRenderer* StaticMeshRenderer::Instance = nullptr;

StaticMeshRenderer::StaticMeshRenderer()
{
	Shader::Load(
		STATIC_MESH_VERTEX_SHADER,
		STATIC_MESH_PIXEL_SHADER,
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
	m_AmbientTexture = Shader::GetUniformLocation("ambient_texture");
	m_DiffuseTexture = Shader::GetUniformLocation("diffuse_texture");
	m_SpecularTexture = Shader::GetUniformLocation("specular_texture");
	m_VertexMatricies = Shader::GetUniformLocation("vertex_matrices");
	m_NormalMatricies = Shader::GetUniformLocation("normal_matrices");
	m_ProjectionMatrix = Shader::GetUniformLocation("projection_matrix");
	m_CameraPosition = Shader::GetUniformLocation("camera_position");
}

StaticMeshRenderer::~StaticMeshRenderer()
{

}

StaticMeshRenderer* StaticMeshRenderer::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new StaticMeshRenderer();
	}
    return Instance;
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

StaticMeshRenderer* StaticMeshRenderer::GetInstance()
{
    return Instance;
}

void StaticMeshRenderer::Bind()
{
	Shader::Bind();
}

void StaticMeshRenderer::Render(const StaticMesh& mesh)
{
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

	context->LoadConstantMatrix4F(m_VertexMatricies, static_cast<unsigned int>(mesh.Nodes.size()), GFX_FALSE, &vertex_matrices[0][0][0]);
	context->LoadConstantMatrix4F(m_NormalMatricies, static_cast<unsigned int>(mesh.Nodes.size()), GFX_FALSE, &normal_matrices[0][0][0]);
	context->LoadConstantMatrix4F(m_ProjectionMatrix, 1, GFX_FALSE, &Camera3D::GetInstance()->GetProjectionMatrix()[0][0]);

	Vector3F sun_position = (v_matrix * Vector4F(Sun::Position, 1.0f)).xyz();
	context->LoadConstantArray3F(m_SunPosition, 1, &sun_position[0]);
	context->LoadConstantArray3F(m_SunColor, 1, &Sun::Color[0]);
	context->LoadConstantArray3F(m_CameraPosition, 1, &Camera3D::GetInstance()->GetViewMatrix()[3][0]);

	mesh.GetAmbientTexture()->Bind(m_AmbientTexture, 0);
	mesh.GetDiffuseTexture()->Bind(m_DiffuseTexture, 1);
	mesh.GetSpecularTexture()->Bind(m_SpecularTexture, 2);
    
    context->DrawElements(GFX_TRIANGLES, mesh.GetElementCount(), GFX_TYPE_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
}
