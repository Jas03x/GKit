#ifndef GK_BMP_READER_H
#define GK_BMP_READER_H

#include <gk/core/importer/bitmap.hpp>

namespace BMP_Reader
{
    bool Read(const char* path, Bitmap& bitmap);
}

#endif // GK_BMP_READER_H
