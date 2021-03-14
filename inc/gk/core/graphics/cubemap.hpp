#ifndef GK_CUBEMAP_H
#define GK_CUBEMAP_H

#include <gk/core/graphics/graphics.hpp>
#include <gk/core/graphics/texture.hpp>
#include <gk/core/importer/bitmap.hpp>

class CubeMap
{
private:
	GFX_HANDLE m_Texture;

protected:
	void Init(unsigned int width, unsigned int height, GFX_HANDLE component_format, GFX_HANDLE component_type);

	bool Load(const Bitmap* bitmaps); // +x, -x, +y, -y, +z, -z

public:
	CubeMap();
	CubeMap(const Bitmap* bitmaps); // +x, -x, +y, -y, +z, -z
	~CubeMap();

	void Bind(unsigned int uniform, int target) const;
	
	GFX_HANDLE GetHandle() const;
};

#endif // GK_CUBEMAP_H
