#include <gk/skybox.hpp>

#include <gk/tga_reader.hpp>

Skybox::Skybox() : CubeMap()
{
}

Skybox::Skybox(const char* textures[6])
{
	Bitmap bitmaps[6];
	for (unsigned int i = 0; i < 6; i++)
	{
		TGA_Reader::Read(textures[i], bitmaps[i]);
	}
	this->Load(bitmaps);
}

Skybox::~Skybox()
{
}
