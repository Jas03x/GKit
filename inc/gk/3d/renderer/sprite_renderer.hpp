#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <vector>

#include <gk/3d/graphics/graphics.hpp>
#include <gk/3d/graphics/shader.hpp>
#include <gk/3d/core/sprite.hpp>
#include <gk/math/transform_2d.hpp>

class SpriteRenderer : public Shader
{
private:
	static SpriteRenderer* Instance;

private:
	GFX_HANDLE m_MatrixLocation;
	GFX_HANDLE m_SpriteLocation;

private:
	SpriteRenderer();
	~SpriteRenderer();

public:
	static SpriteRenderer* CreateInstance();
	static void DeleteInstance();
    
    static SpriteRenderer* GetInstance();

public:
	void Bind();
	void Render(const Sprite& sprite);
	void Render(const Sprite& sprite, const std::vector<Transform2D>& instances);
};

#endif // SPRITE_RENDERER_H
