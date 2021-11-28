#ifndef GK_BITMAP_HPP
#define GK_BITMAP_HPP

#include <cstdint>

struct Bitmap
{
public:
	enum Format : uint8_t
	{
		GREYSCALE = 0,
		RGB		  = 1,
		RGBA      = 2
	};

public:
	uint32_t width;
	uint32_t height;
	uint8_t  bpp;
	uint8_t* pixels;

	Format format;

public:
    static bool Load(const char* path, Bitmap& bitmap);
    
public:
	Bitmap();
	Bitmap(const char* path);
	~Bitmap();
};

#endif // GK_BITMAP_HPP
