#include <gk/sprite.hpp>

#include <gk/tga_image.hpp>

Sprite::Sprite()
{
}

Sprite::Sprite(const char* path)
{
	TgaImage image(path);
	Load(image.HasAlpha() ? GFX_RGBA : GFX_RGB, image.GetWidth(), image.GetHeight(), GFX_TYPE_UNSIGNED_BYTE, image.GetPixels(), GFX_LINEAR, GFX_CLAMP_TO_EDGE);
	
	this->Transform.Size = Vector2F(static_cast<float>(image.GetWidth()), static_cast<float>(image.GetHeight())) * 0.5f;
	this->Transform.Position = Vector2F(0.0f);
	this->Transform.Rotation = 0.0f;
}

Sprite::~Sprite()
{
}
