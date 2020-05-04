#include <gk/tga_reader.hpp>

#include <assert.h>
#include <string.h>

#include <gk/file.hpp>

// TGA Loader
// ONLY SUPPORTS COMPRESSED TGA FILES

// Compressed TGA header:
const uint8_t CMP_TGA_HDR[] = {
	0x00, 0x00, 0x0A, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

typedef struct TGA_Header
{
	uint8_t id_size;
	uint8_t color_type;
	uint8_t image_type;
	uint8_t color_spec[5];
	uint8_t x_origin[2];
	uint8_t y_origin[2];
	uint8_t width[2];
	uint8_t height[2];
	uint8_t bpp;
	uint8_t image_desc;
} TGA_Header;

bool TGA_Reader::Read(const char* path, Bitmap& bitmap)
{
	File file(path, File::READ_BINARY);

	TGA_Header header;
	file.Read(&header, sizeof(TGA_Header), 1);
	
	assert(memcmp(CMP_TGA_HDR, &header, sizeof(CMP_TGA_HDR)) == 0);

	bitmap.width = header.width[1] * 256 + header.width[0];
	bitmap.height = header.height[1] * 256 + header.height[0];
	uint32_t bpp = header.bpp;
	bitmap.has_alpha = (bpp == 32);

	assert((bitmap.width > 0) && (bitmap.height > 0) && ((bpp == 24) || (bpp == 32)));

	uint32_t pixel_size = bpp / 8;
	uint32_t pixel_count = bitmap.width * bitmap.height;
	uint8_t* pixel = new uint8_t[pixel_size];
	
	bitmap.pixels = new uint8_t[pixel_size * pixel_count];

	for (uint32_t p = 0; p < pixel_count; )
	{
		uint8_t chunk_header = 0;
		file.Read(&chunk_header, sizeof(uint8_t), 1);

		if (chunk_header < 128)
		{
			for (uint32_t i = 0; i <= chunk_header; i++)
			{
				file.Read(pixel, pixel_size, 1);
				bitmap.pixels[p * pixel_size + 0] = pixel[2];
				bitmap.pixels[p * pixel_size + 1] = pixel[1];
				bitmap.pixels[p * pixel_size + 2] = pixel[0];
				if (bitmap.has_alpha)
				{
					bitmap.pixels[p * pixel_size + 3] = pixel[3];
				}
				p ++;
			}
		}
		else
		{
			chunk_header -= 128;
			file.Read(pixel, pixel_size, 1);
			for (uint32_t i = 0; i <= chunk_header; i++)
			{
				bitmap.pixels[p * pixel_size + 0] = pixel[2];
				bitmap.pixels[p * pixel_size + 1] = pixel[1];
				bitmap.pixels[p * pixel_size + 2] = pixel[0];
				if (bitmap.has_alpha)
				{
					bitmap.pixels[p * pixel_size + 3] = pixel[3];
				}
				p++;
			}
		}
	}

	delete[] pixel;
	return true;
}
