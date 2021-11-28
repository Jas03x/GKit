#ifndef GK_CUBEMAP_TEXTURE_H
#define GK_CUBEMAP_TEXTURE_H

#include <gk/core/graphics/graphics.hpp>
#include <gk/core/graphics/texture.hpp>

class CubeMapTexture : public Texture
{
private:
	GFX_HANDLE m_Handle;

protected:
	void Load(uint32_t format[6], unsigned int width[6], unsigned int height[6], uint32_t type[6], void* pixels[6], uint32_t filter_mode, uint32_t wrap_mode);

public:
	enum FACE
	{
		POSITIVE_X = 0,
		NEGATIVE_X = 1,
		POSITIVE_Y = 2,
		NEGATIVE_Y = 3,
		POSITIVE_Z = 4,
		NEGATIVE_Z = 5,
		COUNT = 6
	};

public:
	CubeMapTexture();
	CubeMapTexture(const Bitmap* bitmaps, uint32_t filter_mode, uint32_t wrap_mode);
	CubeMapTexture(uint32_t format[6], unsigned int width[6], unsigned int height[6], uint32_t type[6], void* pixels[6], uint32_t filter_mode, uint32_t wrap_mode);
	~CubeMapTexture();

	void Bind(uint32_t uniform, int target) const;
};

#endif // GK_CUBEMAP_TEXTURE_H
