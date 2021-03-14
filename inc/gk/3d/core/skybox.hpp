#ifndef GK_SKYBOX_H
#define GK_SKYBOX_H

#include <gk/graphics/cubemap.hpp>

class Skybox : public CubeMap
{
public:
	Skybox();
	Skybox(const char* textures[6]); // +x, -x, +y, -y, +z, -z
	~Skybox();
};

#endif // GK_SKYBOX_H