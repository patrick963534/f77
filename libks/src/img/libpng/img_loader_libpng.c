#include "img_loader_libpng.h"
#include <ks/log.h>
#include <stdlib.h>
#include <png.h>

void so_img_loader_libpng_save(const char* pixels, int width, int height)
{
    FILE* save_fp = fopen("tt.ppm", "wb");
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

void so_img_loader_libpng_load(const char* file, so_img_loader_data_t* info)
{
    png_structp png_ptr;
    png_infop   info_ptr;
    png_bytep*  row_pointers;
    FILE* fp;
    int i;

    if (NULL == (fp = fopen(file, "rb")))
    {
        ks_log("can not find file: %s", file);
        return;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    info->width = png_get_image_width(png_ptr, info_ptr);
    info->height = png_get_image_height(png_ptr, info_ptr);

    if (png_get_bit_depth(png_ptr, info_ptr) == 16)
        return;

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * info->height);
    info->pixels = malloc(info->width * info->height * 4);

    for (i = 0; i < info->height; ++i)
        row_pointers[i] = (png_byte*)&info->pixels[i * info->width * 4];

    png_read_image(png_ptr, row_pointers);

    ks_log("load image: %s, with size(%d, %d)", file, info->width, info->height);

    fclose(fp);
}
