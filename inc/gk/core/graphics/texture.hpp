#ifndef GK_TEXTURE_H
#define GK_TEXTURE_H

#include <gk/core/graphics/graphics.hpp>

class Texture
{
private:
	GFX_HANDLE m_Handle;

protected:
	void Load(uint32_t format, unsigned int width, unsigned int height, uint32_t type, void* pixels, uint32_t filter_mode, uint32_t wrap_mode);

public:
	Texture();
	Texture(uint32_t format, unsigned int width, unsigned int height, uint32_t type, void* pixels, uint32_t filter_mode, uint32_t wrap_mode);
	~Texture();

	void Bind(uint32_t uniform, int target) const;
	GFX_HANDLE GetHandle() const;
};

#endif // GK_TEXTURE_H
