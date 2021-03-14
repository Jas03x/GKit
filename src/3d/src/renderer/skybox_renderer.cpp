#include <gk/3d/renderer/skybox_renderer.hpp>

#include <cassert>

#include <gk/3d/config/config.hpp>
#include <gk/3d/core/camera_3d.hpp>
#include <gk/graphics/quad.hpp>
#include <gk/3d/io/file.hpp>
#include <gk/math/linear.hpp>

#include "../../shdr/skybox_pixel_shader.hpp"
#include "../../shdr/skybox_vertex_shader.hpp"

SkyboxRenderer* SkyboxRenderer::Instance = nullptr;

SkyboxRenderer::SkyboxRenderer()
{
	Shader::Load(
		SKYBOX_VERTEX_SHADER,
		SKYBOX_PIXEL_SHADER,
		[](GFX_HANDLE id) {
			RenderingContext::GetInstance()->SetAttributeLocation(id, Quad::VertexAttributes::VERTEX, "vertex");
		}
	);

	m_CubeMap = Shader::GetUniformLocation("cubeMap");
	m_InverseViewProjectionMatrix = Shader::GetUniformLocation("inv_vp_matrix");
}

SkyboxRenderer::~SkyboxRenderer()
{

}

SkyboxRenderer* SkyboxRenderer::CreateInstance()
{
	assert(Instance == nullptr);
	if (Instance == nullptr)
	{
		Instance = new SkyboxRenderer();
	}
    return Instance;
}

void SkyboxRenderer::DeleteInstance()
{
	assert(Instance != nullptr);
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

SkyboxRenderer* SkyboxRenderer::GetInstance()
{
    return Instance;
}

void SkyboxRenderer::Bind()
{
	const RenderingContext* context = RenderingContext::GetInstance();

	Shader::Bind();
	context->SetDepthMask(GFX_FALSE);
}

void SkyboxRenderer::Render(const Skybox& skybox)
{
    Quad::GetInstance()->Bind();

	const RenderingContext* context = RenderingContext::GetInstance();
	
	Matrix4F projection = Camera3D::GetInstance()->GetProjectionMatrix();
	Matrix4F view = Camera3D::GetInstance()->GetViewMatrix();
	view[3] = Vector4F(Vector3F(0.0f), 1.0f);
	Matrix4F inv_vp = Matrix::Inverse(projection * view);

	skybox.Bind(m_CubeMap, 0);
	context->LoadConstantMatrix4F(m_InverseViewProjectionMatrix, 1, GFX_FALSE, &inv_vp[0][0]);
	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetInstance()->GetVertexCount());
}
