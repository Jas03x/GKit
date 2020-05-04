#include <gk/bitmap.hpp>

#include <stdio.h>
#include <string.h>

#include <gk/bmp_reader.hpp>
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
	bool status = true;
	size_t len = strlen(path);

	if(len <= 4)
	{
		status = false;
		printf("invalid bitmap file name\n");
	}

	const char* type = nullptr;
	if(status)
	{
		type = path + (len - 4);
	}

	if(status)
	{
		if(strcmp(type, ".bmp") == 0)
		{
			status = BMP_Reader::Read(path, *this);
		}
		else if(strcmp(type, ".tga") == 0)
		{
			status = TGA_Reader::Read(path, *this);
		}
	}
}

Bitmap::~Bitmap()
{
	if (pixels != nullptr)
	{
		delete[] pixels;
		pixels = nullptr;
	}
}
