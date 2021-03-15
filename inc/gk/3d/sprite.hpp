#ifndef GK_SPRITE_H
#define GK_SPRITE_H

#include <gk/core/math/linear.hpp>
#include <gk/core/graphics/graphics.hpp>
#include <gk/core/graphics/texture.hpp>
#include <gk/core/math/transform_2d.hpp>

class Sprite : public Texture
{
public:
	Transform2D Transform;

	Sprite();
	Sprite(const char* path);
	~Sprite();
};

#endif // GK_SPRITE_H
