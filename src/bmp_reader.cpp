#include <gk/bmp_reader.hpp>

#include <stdint.h>

#include <gk/file.hpp>

// bmp file format: https://en.wikipedia.org/wiki/BMP_file_format

const uint32_t BMP_SIGNATURE = 0x4D42;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
struct BMP_HDR
{
    uint16_t type; // 'BM'
    uint32_t size; // size of file
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset; // offset to pixel data
};

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header
struct BMP_DESC
{
    uint32_t type; // type (based off size of this descriptor) = 124
    uint32_t width; // width of this image
    uint32_t height; // height of this image
    uint16_t planes; // number of planes = 1
    uint16_t bpp; // bits per pixel
    uint32_t compression;
    uint32_t size;
    uint32_t x_resolution; // horizontal resolution - pixels per meter
    uint32_t y_resolution; // vertical resolution - pixels per meter
    uint32_t clr_used;
    uint32_t clr_important;
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t alpha_mask;
    uint32_t color_space_type;
    struct{
        struct { uint32_t x, y, z; } red;
        struct { uint32_t x, y, z; } green;
        struct { uint32_t x, y, z; } blue;
    } endpoints;
    uint32_t gamma_red;
    uint32_t gamma_green;
    uint32_t gamma_blue;
    uint32_t intent;
    uint32_t profile_data;
    uint32_t profile_size;
    uint32_t reserved;
};

bool BMP_Reader::Read(const char* path, Bitmap& bitmap)
{    
    bool status = true;

    File file(path, File::READ_BINARY);
    if(!file.IsOpen()) {
        status = false;
    }

    BMP_HDR hdr = {};
    if(status)
    {
        status = file.Read(&hdr, sizeof(BMP_HDR), 1);
    }

    if(status)
    {
        if(hdr.type != BMP_SIGNATURE)
        {
            status = false;
            printf("file is not a bitmap file\n");
        }
    }

    BMP_DESC desc = {};
    if(status)
    {
        status = file.Read(&desc, sizeof(BMP_DESC), 1);
    }

    if(status)
    {
        if(desc.type != 0x7C)
        {
            status = false;
            printf("unsupported bitmap file\n");
        }
    }

    if(status)
    {
    }

    printf("Header size = %lu\n", sizeof(BMP_HDR));
    printf("Descriptor size = %lu\n", sizeof(BMP_DESC));
    printf("Combined size = %lu\n", sizeof(BMP_HDR) + sizeof(BMP_DESC));

    printf("\nHDR:\n");
    printf("Type: 0x%hX\n", hdr.type);
    printf("Size: 0x%u\n", hdr.size);
    printf("Offset: 0x%X\n", hdr.offset);

    return status;
}
