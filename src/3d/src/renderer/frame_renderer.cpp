#include <gk/3d/renderer/frame_renderer.hpp>

#include <cassert>

#include <gk/3d/config.hpp>
#include <gk/core/graphics/quad.hpp>
#include <gk/core/io/file.hpp>

#include "../../shdr/frame_pixel_shader.hpp"
#include "../../shdr/frame_vertex_shader.hpp"

FrameRenderer* FrameRenderer::Instance = nullptr;

FrameRenderer::FrameRenderer()
{
    Shader::Load(
		FRAME_VERTEX_SHADER,
		FRAME_PIXEL_SHADER,
		[](GFX_HANDLE id) {
			RenderingContext::GetInstance()->SetAttributeLocation(id, Quad::VertexAttributes::VERTEX, "vertex");
		}
	);

	m_Opacity = Shader::GetUniformLocation("opacity");
	m_TextureID = Shader::GetUniformLocation("textureID");
}

FrameRenderer::~FrameRenderer()
{

}

FrameRenderer* FrameRenderer::CreateInstance()
{
	assert(Instance == nullptr);
    if (Instance == nullptr)
	{
		Instance = new FrameRenderer();
	}
    return Instance;
}

void FrameRenderer::DeleteInstance()
{
	assert(Instance != nullptr);
    if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

FrameRenderer* FrameRenderer::GetInstance()
{
    return Instance;
}

void FrameRenderer::Bind()
{
	Shader::Bind();
	Quad::GetInstance()->Bind();
}

void FrameRenderer::Render(GFX_HANDLE textureID, float opacity)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	context->LoadConstant1F(m_Opacity, opacity);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(Instance->m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetInstance()->GetVertexCount());
}
