#include <gk/bitmap.hpp>

#include <gk/tga_reader.hpp>

Bitmap::Bitmap()
{
	width = 0;
	height = 0;
	pixels = nullptr;
	flags = 0;
}

Bitmap::Bitmap(const char* path)
{
	TGA_Reader::Read(path, *this);
}

Bitmap::~Bitmap()
{
	if (pixels != nullptr)
	{
		delete[] pixels;
		pixels = nullptr;
	}
}
