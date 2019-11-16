#ifndef GK_TGA_IMAGE_H
#define GK_TGA_IMAGE_H

#include <stdint.h>

class TgaImage
{
private:
	uint8_t *m_Pixels;
	uint32_t m_Width;
	uint32_t m_Height;

	union
	{
		struct
		{
			uint8_t m_HasAlpha : 1;
		};

		uint8_t m_Flags;
	};

public:
	TgaImage(const char* path);
	~TgaImage();

	uint8_t* GetPixels();
	uint32_t GetWidth();
	uint32_t GetHeight();
	bool	 HasAlpha();
};

#endif // GK_TGA_IMAGE_H