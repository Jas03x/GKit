#include <gk/frame_renderer.hpp>

#include <assert.h>

#include <gk/config.hpp>
#include <gk/file.hpp>
#include <gk/quad.hpp>

#include "../shdr/frame_pixel_shader.hpp"
#include "../shdr/frame_vertex_shader.hpp"

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

void FrameRenderer::CreateInstance()
{
	assert(Instance == nullptr);
    if (Instance == nullptr)
	{
		Instance = new FrameRenderer();
	}
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

void FrameRenderer::Bind()
{
    assert(Instance != nullptr);
    
	Instance->Shader::Bind();
	Quad::Bind();
}

void FrameRenderer::Render(GFX_HANDLE textureID, float opacity)
{
    assert(Instance != nullptr);
	const RenderingContext* context = RenderingContext::GetInstance();

	context->LoadConstant1F(Instance->m_Opacity, opacity);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(Instance->m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetVertexCount());
}
