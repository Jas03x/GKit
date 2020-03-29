#include <gk/bitmap.hpp>

#include <gk/tga_image.hpp>

Bitmap::Bitmap()
{
	width = 0;
	height = 0;
	pixels = nullptr;
	flags = 0;
}

Bitmap::Bitmap(const char* path)
{
	ReadTGA(path, *this);
}

Bitmap::~Bitmap()
{
	if (pixels != nullptr)
	{
		delete[] pixels;
		pixels = nullptr;
	}
}
