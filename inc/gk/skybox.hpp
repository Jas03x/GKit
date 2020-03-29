#ifndef GK_SKYBOX_H
#define GK_SKYBOX_H

#include <gk/graphics.hpp>
#include <gk/texture.hpp>
#include <gk/bitmap.hpp>

class Skybox
{
private:
	GFX_HANDLE m_Texture;

protected:
	bool Load(const Bitmap* bitmaps); // +x, -x, +y, -y, +z, -z

public:
	Skybox();
	Skybox(const char* textures[6]); // +x, -x, +y, -y, +z, -z
	~Skybox();

	void Bind(unsigned int uniform, int target) const;
};

#endif // GK_SKYBOX_H