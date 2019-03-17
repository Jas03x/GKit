#include <gk/tga_image.hpp>

#include <string.h>

#include <gk/assert.hpp>
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
}TGA_Header;

TgaImage::TgaImage(const char* path)
{
	File* file = File::Open(path, "rb");
	GK_ASSERT(file != nullptr, ("Error: Could not open file [%s] for reading\n", path));

	TGA_Header header;
	file->Read(&header, sizeof(TGA_Header), 1);
	
	GK_ASSERT(memcmp(CMP_TGA_HDR, &header, sizeof(CMP_TGA_HDR)) == 0, ("Invalid TGA file %s\n", path));

	m_Width = header.width[1] * 256 + header.width[0];
	m_Height = header.height[1] * 256 + header.height[0];
	uint32_t bpp = header.bpp;
	m_HasAlpha = (bpp == 32);

	GK_ASSERT((m_Width > 0) && (m_Height > 0) && ((bpp == 24) || (bpp == 32)), ("Corrupt TGA file %s\n", path));

	uint32_t pixel_size = bpp / 8;
	uint32_t pixel_count = m_Width * m_Height;
	uint8_t* pixel = new uint8_t[pixel_size];
	
	m_Pixels = new uint8_t[pixel_size * pixel_count];

	for (uint32_t p = 0; p < pixel_count; )
	{
		uint8_t chunk_header = 0;
		file->Read(&chunk_header, sizeof(uint8_t), 1);

		if (chunk_header < 128)
		{
			for (uint32_t i = 0; i <= chunk_header; i++)
			{
				file->Read(pixel, pixel_size, 1);
				m_Pixels[p * pixel_size + 0] = pixel[2];
				m_Pixels[p * pixel_size + 1] = pixel[1];
				m_Pixels[p * pixel_size + 2] = pixel[0];
				if (m_HasAlpha)
				{
					m_Pixels[p * pixel_size + 3] = pixel[3];
				}
				p ++;
			}
		}
		else
		{
			chunk_header -= 128;
			file->Read(pixel, pixel_size, 1);
			for (uint32_t i = 0; i <= chunk_header; i++)
			{
				m_Pixels[p * pixel_size + 0] = pixel[2];
				m_Pixels[p * pixel_size + 1] = pixel[1];
				m_Pixels[p * pixel_size + 2] = pixel[0];
				if (m_HasAlpha)
				{
					m_Pixels[p * pixel_size + 3] = pixel[3];
				}
				p++;
			}
		}
	}

	delete file;
	delete[] pixel;
}

TgaImage::~TgaImage()
{
	if (m_Pixels)
	{
		delete[] m_Pixels;
	}
}

uint8_t* TgaImage::GetPixels() {
	return m_Pixels;
}

uint32_t TgaImage::GetWidth() {
	return m_Width;
}

uint32_t TgaImage::GetHeight() {
	return m_Height;
}

bool TgaImage::HasAlpha() {
	return (m_HasAlpha == 1);
}
