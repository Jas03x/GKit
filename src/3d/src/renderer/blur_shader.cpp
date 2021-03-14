#include <gk/3d/renderer/blur_shader.hpp>

#include <cassert>

#include <gk/3d/config/config.hpp>
#include <gk/3d/io/file.hpp>
#include <gk/graphics/quad.hpp>
#include <gk/math/linear.hpp>

#include "../../shdr/gaussian_blur_pixel_shader.hpp"
#include "../../shdr/gaussian_blur_vertex_shader.hpp"

BlurShader* BlurShader::Instance = nullptr;

BlurShader::BlurShader()
{
    Shader::Load(
		GAUSSIAN_BLUR_VERTEX_SHADER,
		GAUSSIAN_BLUR_PIXEL_SHADER,
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

BlurShader* BlurShader::CreateInstance()
{
	assert(Instance == nullptr);
    if (Instance == nullptr)
	{
		Instance = new BlurShader();
	}
    return Instance;
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

BlurShader* BlurShader::GetInstance()
{
    return Instance;
}

void BlurShader::Bind()
{
	Shader::Bind();
	Quad::GetInstance()->Bind();
}

void BlurShader::VerticalBlur(GFX_HANDLE textureID, float height)
{
	const RenderingContext* context = RenderingContext::GetInstance();
	const Vector2F direction = Vector2F(0.0f, 1.0f);

	context->LoadConstantArray2F(m_Direction, 1, &direction[0]);
	context->LoadConstant1F(m_Length, height);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetInstance()->GetVertexCount());
}

void BlurShader::HorizontalBlur(GFX_HANDLE textureID, float width)
{
	const RenderingContext* context = RenderingContext::GetInstance();
	const Vector2F direction = Vector2F(1.0f, 0.0f);

	context->LoadConstantArray2F(m_Direction, 1, &direction[0]);
	context->LoadConstant1F(m_Length, width);
	
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0);
	context->BindTexture(GFX_TEXTURE_2D, textureID);
	context->LoadConstant1I(m_TextureID, 0);

	context->DrawArray(GFX_TRIANGLES, 0, Quad::GetInstance()->GetVertexCount());
}
