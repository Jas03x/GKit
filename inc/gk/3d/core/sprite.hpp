#ifndef GK_SPRITE_H
#define GK_SPRITE_H

#include <gk/3d/math/linear.hpp>
#include <gk/3d/graphics/graphics.hpp>
#include <gk/3d/graphics/texture.hpp>
#include <gk/3d/math/transform_2d.hpp>

class Sprite : public Texture
{
public:
	Transform2D Transform;

	Sprite();
	Sprite(const char* path);
	~Sprite();
};

#endif // GK_SPRITE_H
