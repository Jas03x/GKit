#include <gk/core/graphics/cubemap_texture.hpp>

CubeMapTexture::CubeMapTexture()
{
	m_Handle = 0;
}

CubeMapTexture::CubeMapTexture(const Bitmap* bitmaps, uint32_t filter_mode, uint32_t wrap_mode)
{
	uint32_t format[6] = {};
	uint32_t width[6] = {};
	uint32_t height[6] = {};
	uint32_t type[6] = {};
	void*    pixels[6] = {};

	for (unsigned int i = 0; i < FACE::COUNT; i++)
	{
		format[i] = GetColorFormat(bitmaps[i].format);
		width[i] = bitmaps[i].width;
		height[i] = bitmaps[i].height;
		type[i] = GFX_TYPE_UNSIGNED_BYTE;
		pixels[i] = bitmaps[i].pixels;
	}

	Load(format, width, height, type, pixels, filter_mode, wrap_mode);
}

CubeMapTexture::CubeMapTexture(uint32_t format[6], unsigned int width[6], unsigned int height[6], uint32_t type[6], void* pixels[6], uint32_t filter_mode, uint32_t wrap_mode)
{
	Load(format, width, height, type, pixels, filter_mode, wrap_mode);
}

CubeMapTexture::~CubeMapTexture()
{
	const RenderingContext* context = RenderingContext::GetInstance();
	if (context->IsTexture(m_Handle) == GFX_TRUE)
	{
		context->DeleteTextures(1, &m_Handle);
	}
}

void CubeMapTexture::Load(uint32_t format[6], unsigned int width[6], unsigned int height[6], uint32_t type[6], void* pixels[6], uint32_t filter_mode, uint32_t wrap_mode)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	context->CreateTextures(1, &m_Handle);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Handle);

	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format[FACE::POSITIVE_X], width[FACE::POSITIVE_X], height[FACE::POSITIVE_X], 0, format[FACE::POSITIVE_X], type[FACE::POSITIVE_X], pixels[FACE::POSITIVE_X]);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format[FACE::NEGATIVE_X], width[FACE::NEGATIVE_X], height[FACE::NEGATIVE_X], 0, format[FACE::NEGATIVE_X], type[FACE::NEGATIVE_X], pixels[FACE::NEGATIVE_X]);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format[FACE::POSITIVE_Y], width[FACE::POSITIVE_Y], height[FACE::POSITIVE_Y], 0, format[FACE::POSITIVE_Y], type[FACE::POSITIVE_Y], pixels[FACE::POSITIVE_Y]);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format[FACE::NEGATIVE_Y], width[FACE::NEGATIVE_Y], height[FACE::NEGATIVE_Y], 0, format[FACE::NEGATIVE_Y], type[FACE::NEGATIVE_Y], pixels[FACE::NEGATIVE_Y]);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format[FACE::POSITIVE_Z], width[FACE::POSITIVE_Z], height[FACE::POSITIVE_Z], 0, format[FACE::POSITIVE_Z], type[FACE::POSITIVE_Z], pixels[FACE::POSITIVE_Z]);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format[FACE::NEGATIVE_Z], width[FACE::NEGATIVE_Z], height[FACE::NEGATIVE_Z], 0, format[FACE::NEGATIVE_Z], type[FACE::NEGATIVE_Z], pixels[FACE::NEGATIVE_Z]);

	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_MIN_FILTER, filter_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_MAG_FILTER, filter_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_WRAP_S, wrap_mode);
	context->SetTextureParameter(GFX_TEXTURE_2D, GFX_TEXTURE_WRAP_T, wrap_mode);
}

void CubeMapTexture::Bind(uint32_t uniform, int target) const
{
	const RenderingContext* context = RenderingContext::GetInstance();

	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0 + target);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Handle);
	context->LoadConstant1I(uniform, target);
}
