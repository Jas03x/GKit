#ifndef GK_TGA_IMAGE_H
#define GK_TGA_IMAGE_H

#include <gk/bitmap.hpp>

bool ReadTGA(const char* path, Bitmap& bitmap);

#endif // GK_TGA_IMAGE_H