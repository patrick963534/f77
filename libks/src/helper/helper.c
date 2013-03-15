#include <ks/helper.h>
#include <stdio.h>
#include <stdlib.h>
#include "ks/environment.h"
#include "ks/path.h"

KS_API void ks_helper_int_to_bytes(char* buf, int val)
{
    buf[0] = (char)((val >> 24));
    buf[1] = (char)((val >> 16) & 0xFF);
    buf[2] = (char)((val >>  8) & 0xFF);
    buf[3] = (char)((val      ) & 0xFF);
}

KS_API void ks_helper_bytes_to_int(const char* buf, int* val)
{
    const unsigned char* b = (const unsigned char*)buf;

    *val = (int)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
}

KS_API int ks_helper_file_get_length(const char* file)
{
    FILE*   fp;
    int    len;

    fp = fopen(file, "rb");
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fclose(fp);

    return len;
}

KS_API char* ks_helper_file_read_all(const char* file, int* ret_sz)
{
    FILE*   fp;
    int     len;
    char*   data;

    fp = fopen(file, "rb");
    if (!fp)
        return 0;

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);

    data = malloc(len);
    *ret_sz = len;

    fseek(fp, 0, SEEK_SET);
    fread(data, len, 1, fp);
    fclose(fp);

    return data;
}

KS_API int ks_helper_file_exist(const char* file)
{
    FILE* fp;
    int   ret = 0;

    fp = fopen(file, "rb");

    if (fp)
    {
        ret = 1;
        fclose(fp);
    }

    return ret;
}

KS_API char* ks_helper_path_join_relative_app(char* buf, int sz, const char* filename)
{
    char folder[256];

    ks_path_folder(folder, sizeof(folder), ks_environment_app_filepath());
    ks_path_join(buf, sz, folder, filename);

    return buf;
}

KS_API void ks_helper_image_save_ppm(const char* file, const char* pixels, int width, int height)
{
    FILE* save_fp = fopen(file, "wb");
    char* buffer = malloc(width * 3);
    int i, j;

    fprintf(save_fp, "P6 %d %d 255 ", width, height);

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            buffer[j * 3]    = pixels[i * width * 4 + j * 4];
            buffer[j * 3+ 1] = pixels[i * width * 4 + j * 4 + 1];
            buffer[j * 3+ 2] = pixels[i * width * 4 + j * 4 + 2];
        }

        fwrite(buffer, 1, width * 3, save_fp);
    }

    fclose(save_fp);
}

KS_API void ks_helper_image565_save_ppm(const char* file, const char* pixels, int width, int height)
{
    FILE* save_fp = fopen(file, "wb");
    char* buffer = malloc(width * 3);
    unsigned short* sp = (unsigned short*)pixels;
    int i, j;

    fprintf(save_fp, "P6 %d %d 255 ", width, height);

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int v = sp[i * width + j];

            buffer[j * 3]    = ((v >> 11) & 0x1F) * 255 / 32;
            buffer[j * 3+ 1] = ((v >>  5) & 0x3F) * 255 / 64;
            buffer[j * 3+ 2] = ((v >>  0) & 0x1F) * 255 / 32;
        }

        fwrite(buffer, 1, width * 3, save_fp);
    }
}
