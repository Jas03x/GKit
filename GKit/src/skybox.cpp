#include <gk/skybox.hpp>

#include <gk/tga_image.hpp>

Skybox::Skybox(const char* textures[6])
{
	TgaImage x_pos(textures[0]);
	TgaImage x_neg(textures[1]);
	// Y-axis swapped:
	TgaImage y_pos(textures[3]);
	TgaImage y_neg(textures[2]);
	TgaImage z_pos(textures[4]);
	TgaImage z_neg(textures[5]);

	const RenderingContext* context = RenderingContext::GetInstance();

	context->CreateTextures(1, &m_Texture);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Texture);
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_X, 0, x_pos.HasAlpha() ? GFX_RGBA : GFX_RGB, x_pos.GetWidth(), x_pos.GetHeight(), 0, x_pos.HasAlpha() ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, x_pos.GetPixels());
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, x_neg.HasAlpha() ? GFX_RGBA : GFX_RGB, x_neg.GetWidth(), x_neg.GetHeight(), 0, x_neg.HasAlpha() ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, x_neg.GetPixels());
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, y_pos.HasAlpha() ? GFX_RGBA : GFX_RGB, y_pos.GetWidth(), y_pos.GetHeight(), 0, y_pos.HasAlpha() ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, y_pos.GetPixels());
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, y_neg.HasAlpha() ? GFX_RGBA : GFX_RGB, y_neg.GetWidth(), y_neg.GetHeight(), 0, y_neg.HasAlpha() ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, y_neg.GetPixels());
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, z_pos.HasAlpha() ? GFX_RGBA : GFX_RGB, z_pos.GetWidth(), z_pos.GetHeight(), 0, z_pos.HasAlpha() ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, z_pos.GetPixels());
	context->CreateTexture2D(GFX_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, z_neg.HasAlpha() ? GFX_RGBA : GFX_RGB, z_neg.GetWidth(), z_neg.GetHeight(), 0, z_neg.HasAlpha() ? GFX_RGBA : GFX_RGB, GFX_TYPE_UNSIGNED_BYTE, z_neg.GetPixels());

	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_MIN_FILTER, GFX_LINEAR);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_MAG_FILTER, GFX_LINEAR);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_S, GFX_CLAMP_TO_EDGE);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_T, GFX_CLAMP_TO_EDGE);
	context->SetTextureParameter(GFX_TEXTURE_CUBE_MAP, GFX_TEXTURE_WRAP_R, GFX_CLAMP_TO_EDGE);
}

Skybox::~Skybox()
{
	const RenderingContext* context = RenderingContext::GetInstance();
	if (context->IsTexture(m_Texture) == GFX_TRUE)
	{
		context->DeleteTextures(1, &m_Texture);
	}
}

void Skybox::Bind(unsigned int uniform, int target) const
{
	const RenderingContext* context = RenderingContext::GetInstance();
	context->ActivateTextureSlot(GFX_TEXTURE_SLOT0 + target);
	context->BindTexture(GFX_TEXTURE_CUBE_MAP, m_Texture);
	context->LoadConstant1I(uniform, target);
}
