#include <gk/core/graphics/texture.hpp>

Texture::Texture()
{
	m_Handle = 0;
}

Texture::Texture(const Bitmap& bitmap, uint32_t filter_mode, uint32_t wrap_mode)
{
	Load(GetColorFormat(bitmap.format), bitmap.width, bitmap.height, GFX_TYPE_UNSIGNED_BYTE, bitmap.pixels, filter_mode, wrap_mode);
}

Texture::Texture(uint32_t format, unsigned int width, unsigned int height, uint32_t type, void* pixels, uint32_t filter_mode, uint32_t wrap_mode)
{
	Load(format, width, height, type, pixels, filter_mode, wrap_mode);
}

Texture::~Texture()
{
	const RenderingContext* context = RenderingContext::GetInstance();
	if (context->IsTexture(m_Handle) == GFX_TRUE)
	{
		context->DeleteTextures(1, &m_Handle);
	}
}

void Texture::Load(uint32_t format, unsigned int width, unsigned int height, uint32_t type, void* pixels, uint32_t filter_mode, uint32_t wrap_mode)
{
	const RenderingContext* context = RenderingContext::GetInstance();

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
	const RenderingContext* context = RenderingContext::GetInstance();

	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0 + target);
	context->BindTexture(GFX_TEXTURE_2D, m_Handle);
	context->LoadConstant1I(uniform, target);
}

GFX_HANDLE Texture::GetHandle() const
{
	return m_Handle;
}

GFX_COLOR Texture::GetColorFormat(Bitmap::Format bmp_format)
{
	GFX_COLOR format = static_cast<GFX_COLOR>(0);
	switch (bmp_format)
	{
		case Bitmap::Format::GREYSCALE:
			format = GFX_COLOR::GFX_GREYSCALE;
			break;
		case Bitmap::Format::RGB:
			format = GFX_COLOR::GFX_RGB;
			break;
		case Bitmap::Format::RGBA:
			format = GFX_COLOR::GFX_RGBA;
			break;
	}
	return format;
}
