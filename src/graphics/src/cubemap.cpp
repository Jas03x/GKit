#include <gk/graphics/cubemap.hpp>

#include <gk/importer/bitmap.hpp>

CubeMap::CubeMap()
{
	m_Texture = 0;
}

CubeMap::CubeMap(const Bitmap* bitmaps)
{
	this->Load(bitmaps);
}

CubeMap::~CubeMap()
{
	const RenderingContext* context = RenderingContext::GetInstance();
	if (context->IsTexture(m_Texture) == GFX_TRUE)
	{
		context->DeleteTextures(1, &m_Texture);
	}
}

void CubeMap::Init(unsigned int width, unsigned int height, GFX_HANDLE component_format, GFX_HANDLE component_type)
{
	const RenderingContext* context = RenderingContext::GetInstance();

	context->CreateTextures(1, &m_Texture);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Texture);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_X, 0, component_format, width, height, 0, component_format, component_type, nullptr);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, component_format, width, height, 0, component_format, component_type, nullptr);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, component_format, width, height, 0, component_format, component_type, nullptr);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, component_format, width, height, 0, component_format, component_type, nullptr);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, component_format, width, height, 0, component_format, component_type, nullptr);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, component_format, width, height, 0, component_format, component_type, nullptr);

	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_MIN_FILTER, GFX_LINEAR);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_MAG_FILTER, GFX_LINEAR);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_S, GFX_CLAMP_TO_EDGE);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_T, GFX_CLAMP_TO_EDGE);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_R, GFX_CLAMP_TO_EDGE);
}

bool CubeMap::Load(const Bitmap* bitmaps)
{
	const Bitmap& x_pos = bitmaps[0];
	const Bitmap& x_neg = bitmaps[1];
	const Bitmap& y_pos = bitmaps[2];
	const Bitmap& y_neg = bitmaps[3];
	const Bitmap& z_pos = bitmaps[4];
	const Bitmap& z_neg = bitmaps[5];

	const RenderingContext* context = RenderingContext::GetInstance();

	context->CreateTextures(1, &m_Texture);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Texture);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_X, 0, x_pos.has_alpha ? GFX_RGBA : GFX_RGB, x_pos.width, x_pos.height, 0, x_pos.has_alpha ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, x_pos.pixels);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, x_neg.has_alpha ? GFX_RGBA : GFX_RGB, x_neg.width, x_neg.height, 0, x_neg.has_alpha ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, x_neg.pixels);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, y_pos.has_alpha ? GFX_RGBA : GFX_RGB, y_pos.width, y_pos.height, 0, y_pos.has_alpha ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, y_pos.pixels);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, y_neg.has_alpha ? GFX_RGBA : GFX_RGB, y_neg.width, y_neg.height, 0, y_neg.has_alpha ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, y_neg.pixels);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, z_pos.has_alpha ? GFX_RGBA : GFX_RGB, z_pos.width, z_pos.height, 0, z_pos.has_alpha ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, z_pos.pixels);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, z_neg.has_alpha ? GFX_RGBA : GFX_RGB, z_neg.width, z_neg.height, 0, z_neg.has_alpha ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, z_neg.pixels);

	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_MIN_FILTER, GFX_LINEAR);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_MAG_FILTER, GFX_LINEAR);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_S, GFX_CLAMP_TO_EDGE);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_T, GFX_CLAMP_TO_EDGE);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_R, GFX_CLAMP_TO_EDGE);

	return true;
}

void CubeMap::Bind(unsigned int uniform, int target) const
{
	const RenderingContext* context = RenderingContext::GetInstance();
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0 + target);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Texture);
	context->LoadConstant1I(uniform, target);
}

GFX_HANDLE CubeMap::GetHandle() const
{
	return m_Texture;
}
