#include <gk/skybox.hpp>

#include <gk/tga_image.hpp>

Skybox::Skybox() : CubeMap()
{
}

Skybox::Skybox(const char* textures[6])
{
	Bitmap bitmaps[6];
	for (unsigned int i = 0; i < 6; i++)
	{
		ReadTGA(textures[i], bitmaps[i]);
	}
	this->Load(bitmaps);
}

Skybox::~Skybox()
{
}
