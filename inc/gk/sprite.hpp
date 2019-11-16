#ifndef GK_SPRITE_H
#define GK_SPRITE_H

#include <gk/matrix.hpp>
#include <gk/graphics.hpp>
#include <gk/texture.hpp>
#include <gk/transform_2d.hpp>

class Sprite : public Texture
{
public:
	Transform2D Transform;

	Sprite();
	Sprite(const char* path);
	~Sprite();
};

#endif // GK_SPRITE_H
