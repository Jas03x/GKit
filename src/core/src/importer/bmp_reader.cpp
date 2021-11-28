#include <gk/core/importer/bmp_reader.hpp>

#include <cstdint>

#include <gk/core/io/file.hpp>

// bmp file format: https://en.wikipedia.org/wiki/BMP_file_format

enum : uint16_t { BMP_SIGNATURE = 0x4D42 };

enum : uint32_t
{
    BMP_RGB = 0x0,
    BMP_BITFIELD = 0x3
};

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
#ifdef _WIN32 
    #pragma pack(push, 1)
#endif
struct BMP_HDR
{
    uint16_t type; // 'BM'
    uint32_t size; // size of file
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset; // offset to pixel data
}
#ifdef _WIN32 
    ;
    #pragma pack(pop)
#else
    __attribute__((packed));
#endif

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
struct BMP_INFO_HDR
{
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
};

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv4header
struct BMP_DESC_V4
{
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
    struct {
        struct { uint32_t x, y, z; } red;
        struct { uint32_t x, y, z; } green;
        struct { uint32_t x, y, z; } blue;
    } endpoints;
    uint32_t gamma_red;
    uint32_t gamma_green;
    uint32_t gamma_blue;
};

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header
struct BMP_DESC_V5
{
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
    struct {
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

enum : uint32_t
{
    BMP_V1_HDR = sizeof(BMP_INFO_HDR) + sizeof(uint32_t),
    BMP_V4_HDR = sizeof(BMP_DESC_V4)  + sizeof(uint32_t),
    BMP_V5_HDR = sizeof(BMP_DESC_V5)  + sizeof(uint32_t),
};

struct BMP_DESC
{
    uint32_t width; // width of this image
    uint32_t height; // height of this image
    uint16_t planes; // number of planes = 1
    uint16_t bpp; // bits per pixel
    uint32_t compression;
};

bool ReadDesc(File& file, BMP_DESC& data)
{
    bool status = true;

    uint32_t hdr_ver = 0;
    if (status)
    {
        status = file.Read(&hdr_ver, 1);
    }

    union
    {
        BMP_INFO_HDR v1;
        BMP_DESC_V4  v4;
        BMP_DESC_V5  v5;
    } desc;

    if (status)
    {
        switch (hdr_ver)
        {
            case BMP_V1_HDR:
            {
                status = file.Read(&desc.v1, sizeof(BMP_INFO_HDR), 1);
                if (status)
                {
                    data.width = desc.v1.width;
                    data.height = desc.v1.height;
                    data.planes = desc.v1.planes;
                    data.bpp = desc.v1.bpp;
                    data.compression = desc.v1.compression;
                }
                break;
            }
            case BMP_V4_HDR:
            {
                status = file.Read(&desc.v4, sizeof(BMP_V4_HDR), 1);
                if (status)
                {
                    data.width = desc.v4.width;
                    data.height = desc.v4.height;
                    data.planes = desc.v4.planes;
                    data.bpp = desc.v4.bpp;
                    data.compression = desc.v4.compression;
                }
                break;
            }
            case BMP_V5_HDR:
            {
                status = file.Read(&desc.v4, sizeof(BMP_V5_HDR), 1);
                if (status)
                {
                    data.width = desc.v5.width;
                    data.height = desc.v5.height;
                    data.planes = desc.v5.planes;
                    data.bpp = desc.v5.bpp;
                    data.compression = desc.v5.compression;
                }
                break;
            }
            default:
            {
                status = false;
                printf("unsupported bitmap file\n");
                break;
            }
        }
    }

    if (status)
    {
        if ((data.compression != BMP_RGB) && (data.compression != BMP_BITFIELD))
        {
            status = false;
            printf("invalid compression\n");
        }
    }

    return status;
}

bool BMP_Reader::Read(const char* path, Bitmap& bitmap)
{    
    bool status = true;

    File file(path, File::READ_BINARY);
    if(!file.IsOpen())
    {
        status = false;
    }

    BMP_HDR hdr = {};
    if(status)
    {
        status = file.Read(&hdr, sizeof(BMP_HDR), 1);
    }

    if(status && (hdr.type != BMP_SIGNATURE))
    {
        status = false;
        printf("file is not a bitmap file\n");
    }

    BMP_DESC desc = {};
    status = ReadDesc(file, desc);

    uint8_t* pixels = nullptr;
    if(status)
    {
        uint32_t pixel_size = (desc.bpp / 8);
        uint32_t pixel_count = desc.width * desc.height;
        
        pixels = new uint8_t[pixel_count * pixel_size];

        if (pixel_size == 1)
        {
            file.Seek(FILE_SET, hdr.offset);

            if (!file.Read(pixels, pixel_count))
            {
                status = false;
                printf("error reading pixel data\n");
            }
        }
        else
        {
            union {
                struct { uint8_t r, g, b, a; };
                uint8_t data[4];
            } buffer;

            bool has_alpha = (desc.bpp == 32);

            file.Seek(FILE_SET, hdr.offset);

            for (unsigned int i = 0; status && (i < pixel_count); i++)
            {
                if (!file.Read(buffer.data, pixel_size))
                {
                    status = false;
                    printf("error reading pixel data\n");
                }

                pixels[i * pixel_size + 0] = buffer.b;
                pixels[i * pixel_size + 1] = buffer.g;
                pixels[i * pixel_size + 2] = buffer.r;
                if (has_alpha)
                {
                    pixels[i * pixel_size + 3] = buffer.a;
                }
            }
        }
    }

    bitmap.width = desc.width;
    bitmap.height = desc.height;
    switch (desc.bpp)
    {
    case 8:
        bitmap.format = Bitmap::Format::GREYSCALE;
        break;
    case 24:
        bitmap.format = Bitmap::Format::RGB;
        break;
    case 32:
        bitmap.format = Bitmap::Format::RGBA;
        break;
    }
    bitmap.bpp = (desc.bpp / 8);
    bitmap.pixels = pixels;

    return status;
}
