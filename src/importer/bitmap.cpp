#include <gk/importer/bitmap.hpp>

#include <cstdio>
#include <cstring>

#include <gk/importer/bmp_reader.hpp>
#include <gk/importer/tga_reader.hpp>

bool Bitmap::Load(const char* path, Bitmap& bitmap)
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
            status = BMP_Reader::Read(path, bitmap);
        }
        else if(strcmp(type, ".tga") == 0)
        {
            status = TGA_Reader::Read(path, bitmap);
        }
    }
    
    return status;
}

Bitmap::Bitmap()
{
	width = 0;
	height = 0;
	pixels = nullptr;
	flags = 0;
}

Bitmap::Bitmap(const char* path)
{
    Bitmap::Load(path, *this);
}

Bitmap::~Bitmap()
{
	if (pixels != nullptr)
	{
		delete[] pixels;
		pixels = nullptr;
	}
}
