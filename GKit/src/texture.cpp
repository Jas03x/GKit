#include <gk/texture.hpp>

Texture::Texture()
{
	m_Handle = 0;
}

Texture::Texture(uint32_t format, unsigned int width, unsigned int height, uint32_t type, void* pixels, uint32_t filter_mode, uint32_t wrap_mode)
{
	Load(format, width, height, type, pixels, filter_mode, wrap_mode);
}

Texture::~Texture()
{
	RenderingContext* context = RenderingContext::GetInstance();
	if (context->IsTexture(m_Handle) == GFX_TRUE)
	{
		context->DeleteTextures(1, &m_Handle);
	}
}

void Texture::Load(uint32_t format, unsigned int width, unsigned int height, uint32_t type, void* pixels, uint32_t filter_mode, uint32_t wrap_mode)
{
	RenderingContext* context = RenderingContext::GetInstance();

	context->CreateTextures(1, &m_Handle);
	context->BindTexture(GFX_TEXTURE_2D, m_Handle);
	context->CreateTexture2D(GFX_TEXTURE_2D, 0, format, width, height, 0, format, type, pixels);

	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_MIN_FILTER, filter_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_MAG_FILTER, filter_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_WRAP_S, wrap_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_WRAP_T, wrap_mode);
}

void Texture::Bind(uint32_t uniform, int target) const
{
	RenderingContext* context = RenderingContext::GetInstance();

	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0 + target);
	context->BindTexture(GFX_TEXTURE_2D, m_Handle);
	context->LoadConstant1I(uniform, target);
}

GFX_HANDLE Texture::GetHandle() const
{
	return m_Handle;
}
