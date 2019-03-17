#ifndef GK_SKYBOX_H
#define GK_SKYBOX_H

#include <gk/graphics.hpp>
#include <gk/texture.hpp>

class Skybox
{
private:
	GFX_HANDLE m_Texture;

public:
	Skybox(const char* textures[6]);
	~Skybox();

	void Bind(uint uniform, int target) const;
};

#endif // GK_SKYBOX_H