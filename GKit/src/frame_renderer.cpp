#include <gk/frame_renderer.hpp>

#include <assert.h>

#include <gk/file.hpp>
#include <gk/quad.hpp>

FrameRenderer* FrameRenderer::m_Instance = nullptr;

FrameRenderer::FrameRenderer()
{
    Shader::Load(
		File::Read(SHADER_BASE "frame_renderer.vert").c_str(),
		File::Read(SHADER_BASE "frame_renderer.frag").c_str(),
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
	assert(m_Instance == nullptr);
    if (m_Instance == nullptr)
	{
		m_Instance = new FrameRenderer();
	}
}

void FrameRenderer::DeleteInstance()
{
	assert(m_Instance != nullptr);
    if (m_Instance)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

void FrameRenderer::Bind()
{
    assert(m_Instance != nullptr);
    
	m_Instance->Shader::Bind();
	Quad::Bind();
}

void FrameRenderer::Render(GFX_HANDLE textureID, float opacity)
{
    assert(m_Instance != nullptr);
	RenderingContext* context = RenderingContext::GetInstance();

	context->LoadConstant1F(m_Instance->m_Opacity, opacity);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(m_Instance->m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetVertexCount());
}
