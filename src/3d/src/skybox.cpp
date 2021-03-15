#include <gk/3d/skybox.hpp>

#include <gk/core/importer/bitmap.hpp>

Skybox::Skybox() : CubeMap()
{
}

Skybox::Skybox(const char* textures[6])
{
	Bitmap bitmaps[6];
	for (unsigned int i = 0; i < 6; i++)
	{
		Bitmap::Load(textures[i], bitmaps[i]);
	}
	this->Load(bitmaps);
}

Skybox::~Skybox()
{
}
