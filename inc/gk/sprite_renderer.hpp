#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <vector>

#include <gk/graphics.hpp>
#include <gk/shader.hpp>
#include <gk/sprite.hpp>
#include <gk/transform_2d.hpp>

class SpriteRenderer : public Shader
{
private:
	static SpriteRenderer* Instance;

	GFX_HANDLE m_MatrixLocation;
	GFX_HANDLE m_SpriteLocation;

private:
	SpriteRenderer();
	~SpriteRenderer();

public:
	static void CreateInstance();
	static void DeleteInstance();

	static void Bind();
	static void Render(const Sprite& sprite);
	static void Render(const Sprite& sprite, const std::vector<Transform2D>& instances);
};

#endif // SPRITE_RENDERER_H