#include <gk/skybox.hpp>

#include <gk/tga_image.hpp>

Skybox::Skybox()
{
}

Skybox::Skybox(const char* textures[6])
{
	Bitmap bitmaps[6];
	for (unsigned int i = 0; i < 6; i++) {
		ReadTGA(textures[i], bitmaps[i]);
	}
	this->Load(bitmaps);
}

Skybox::~Skybox()
{
	const RenderingContext* context = RenderingContext::GetInstance();
	if (context->IsTexture(m_Texture) == GFX_TRUE)
	{
		context->DeleteTextures(1, &m_Texture);
	}
}

bool Skybox::Load(const Bitmap* bitmaps)
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

void Skybox::Bind(unsigned int uniform, int target) const
{
	const RenderingContext* context = RenderingContext::GetInstance();
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0 + target);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Texture);
	context->LoadConstant1I(uniform, target);
}
