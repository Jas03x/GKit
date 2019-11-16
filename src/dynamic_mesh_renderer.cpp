#include <gk/dynamic_mesh_renderer.hpp>

#include <gk/sun.hpp>
#include <gk/file.hpp>
#include <gk/camera_3d.hpp>

#include <assert.h>

DynamicMeshRenderer* DynamicMeshRenderer::Instance = nullptr;

DynamicMeshRenderer::DynamicMeshRenderer()
{
	Shader::Load(
		File::Read(SHADER_BASE "dynamic_mesh_renderer.vert").c_str(),
		File::Read(SHADER_BASE "dynamic_mesh_renderer.frag").c_str(),
		[](GFX_HANDLE id)
		{
			const RenderingContext* context = RenderingContext::GetInstance();

			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::VERTEX,      "vertex");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::NORMAL,      "normal");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::UV,	   	  "uv");
			context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::NODE,        "node");
            context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::BONE_INDEX,  "bone_indices");
            context->SetAttributeLocation(id, DynamicMesh::VertexAttributes::BONE_WEIGHT, "bone_weights");

			context->SetFragmentLocation(id, 0, "fragment_color");
		}
	);

	m_SunColor			= Shader::GetUniformLocation("sun_color");
	m_SunPosition		= Shader::GetUniformLocation("sun_position");
	m_DiffuseTexture	= Shader::GetUniformLocation("diffuse_texture");
	m_VertexMatricies	= Shader::GetUniformLocation("vertex_matrices");
	m_NormalMatricies	= Shader::GetUniformLocation("normal_matrices");
	m_ProjectionMatrix	= Shader::GetUniformLocation("projection_matrix");
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

void DynamicMeshRenderer::Render(const DynamicMesh& mesh)
{
	assert(Instance != nullptr);
	
	const RenderingContext* context = RenderingContext::GetInstance();
	mesh.Bind();

	Matrix4F v_matrix = Camera3D::GetInstance()->GetViewMatrix();
	Matrix4F vertex_matrices[DynamicMesh::BONE_LIMIT];
	Matrix4F normal_matrices[DynamicMesh::BONE_LIMIT];

	const Node& root = mesh.Nodes[0];
	printf("Root = %s\n", root.GetName().c_str());
	
    Matrix4F inverse = Matrix::Inverse(root.GetLocalMatrix());
	//Matrix4F inverse = Matrix4F(1.0f);

	for (unsigned int i = 0; i < mesh.Bones.size(); i++)
	{
        const Bone& bone = mesh.Bones[i];
		//vertex_matrices[i] = Matrix4F(1.0f);
		vertex_matrices[i] = inverse * bone.GetNode()->GetGlobalMatrix() * bone.GetOffsetMatrix();
		//vertex_matrices[i] = bone.GetNode()->GetGlobalMatrix();
		//vertex_matrices[i] = inverse * bone.GetOffsetMatrix();
		normal_matrices[i] = Matrix::Inverse(Matrix::Transpose(vertex_matrices[i]));
	}

	context->LoadConstantMatrix4F(Instance->m_VertexMatricies, mesh.Bones.size(), GFX_FALSE, &vertex_matrices[0][0][0]);
    context->LoadConstantMatrix4F(Instance->m_NormalMatricies, mesh.Bones.size(), GFX_FALSE, &normal_matrices[0][0][0]);

	Matrix4F mvp = Camera3D::GetInstance()->GetProjectionMatrix() * v_matrix;// *mesh.RootNode.GetLocalMatrix();
	context->LoadConstantMatrix4F(Instance->m_ProjectionMatrix, 1, GFX_FALSE, &mvp[0][0]);

	Vector3F sun_position = (v_matrix * Vector4F(Sun::Position, 1.0f)).xyz;
	context->LoadConstantArray3F(Instance->m_SunPosition, 1, &sun_position[0]);
	context->LoadConstantArray3F(Instance->m_SunColor, 1, &Sun::Color[0]);

	mesh.GetDiffuseTexture()->Bind(Instance->m_DiffuseTexture, 0);

	//glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
	context->DrawElements(GFX_TRIANGLES, mesh.GetElementCount(), GFX_TYPE_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
}
