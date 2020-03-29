#ifndef GK_BITMAP_HPP
#define GK_BITMAP_HPP

#include <cstdint>

struct Bitmap
{
public:
	uint32_t width;
	uint32_t height;
	uint8_t* pixels;

	union
	{
		struct
		{
			uint8_t has_alpha : 1;
		};

		uint8_t flags;
	};

public:
	Bitmap();
	Bitmap(const char* path);
	~Bitmap();
};

#endif // GK_BITMAP_HPP