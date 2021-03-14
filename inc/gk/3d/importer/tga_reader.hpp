#ifndef GK_TGA_READER_H
#define GK_TGA_READER_H

#include <gk/3d/importer/bitmap.hpp>

namespace TGA_Reader
{
    bool Read(const char* path, Bitmap& bitmap);
}

#endif // GK_TGA_READER_H
