#ifndef GK_CUBEMAP_H
#define GK_CUBEMAP_H

#include <gk/core/graphics/graphics.hpp>
#include <gk/core/importer/bitmap.hpp>
#include <gk/core/graphics/cubemap_texture.hpp>

class CubeMap
{
private:
	CubeMapTexture* m_Texture;

protected:
	bool Load(const Bitmap* bitmaps); // +x, -x, +y, -y, +z, -z

public:
	enum Face
	{
		Positive_X = CubeMapTexture::FACE::POSITIVE_X,
		Negative_X = CubeMapTexture::FACE::NEGATIVE_X,
		Positive_Y = CubeMapTexture::FACE::POSITIVE_Y,
		Negative_Y = CubeMapTexture::FACE::NEGATIVE_Y,
		Positive_Z = CubeMapTexture::FACE::POSITIVE_Z,
		Negative_Z = CubeMapTexture::FACE::NEGATIVE_Z,
		Count = CubeMapTexture::FACE::COUNT
	};

public:
	CubeMap();
	CubeMap(const Bitmap* bitmaps); // +x, -x, +y, -y, +z, -z
	~CubeMap();

	void Bind(unsigned int uniform, int target) const;
	
	CubeMapTexture* GetTexture() const;
};

#endif // GK_CUBEMAP_H
