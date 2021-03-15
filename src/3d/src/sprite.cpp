#include <gk/3d/sprite.hpp>

#include <gk/core/importer/bitmap.hpp>

Sprite::Sprite()
{
}

Sprite::Sprite(const char* path)
{
	Bitmap image(path);
	Load(image.has_alpha ? GFX_RGBA : GFX_RGB, image.width, image.height, GFX_TYPE_UNSIGNED_BYTE, image.pixels, GFX_LINEAR, GFX_CLAMP_TO_EDGE);
	
	this->Transform.Size = Vector2F(static_cast<float>(image.width), static_cast<float>(image.height)) * 0.5f;
	this->Transform.Position = Vector2F(0.0f);
	this->Transform.Rotation = 0.0f;
}

Sprite::~Sprite()
{
}
