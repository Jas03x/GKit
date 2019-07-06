#include <gk/blur_shader.hpp>

#include <assert.h>

#include <gk/file.hpp>
#include <gk/quad.hpp>
#include <gk/matrix.hpp>

BlurShader* BlurShader::Instance = nullptr;

BlurShader::BlurShader()
{
    Shader::Load(
		File::Read(SHADER_BASE "gaussian_blur.vert").c_str(),
		File::Read(SHADER_BASE "gaussian_blur.frag").c_str(),
		[](GFX_HANDLE id) {
			RenderingContext::GetInstance()->SetAttributeLocation(id, Quad::VertexAttributes::VERTEX, "vertex");
		}
	);

	m_TextureID = Shader::GetUniformLocation("textureID");
	m_Direction = Shader::GetUniformLocation("direction");
	m_Length	= Shader::GetUniformLocation("length");
}

BlurShader::~BlurShader()
{

}

void BlurShader::CreateInstance()
{
	assert(Instance == nullptr);

    if (Instance == nullptr)
	{
		Instance = new BlurShader();
	}
}

void BlurShader::DeleteInstance()
{
	assert(Instance != nullptr);
	
    if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void BlurShader::Bind()
{
    assert(Instance != nullptr);
    
	Instance->Shader::Bind();
	Quad::Bind();
}

void BlurShader::VerticalBlur(GFX_HANDLE textureID, float height)
{
    assert(Instance != nullptr);
	
	const RenderingContext* context = RenderingContext::GetInstance();
	const Vector2F direction = Vector2F(0.0f, 1.0f);

	context->LoadConstantArray2F(Instance->m_Direction, 1, &direction[0]);
	context->LoadConstant1F(Instance->m_Length, height);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(Instance->m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetVertexCount());
}

void BlurShader::HorizontalBlur(GFX_HANDLE textureID, float width)
{
    assert(Instance != nullptr);
	
	const RenderingContext* context = RenderingContext::GetInstance();
	const Vector2F direction = Vector2F(1.0f, 0.0f);

	context->LoadConstantArray2F(Instance->m_Direction, 1, &direction[0]);
	context->LoadConstant1F(Instance->m_Length, width);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(Instance->m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetVertexCount());
}
