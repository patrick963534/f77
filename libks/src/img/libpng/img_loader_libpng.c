#include "img_loader_libpng.h"
#include <ks/log.h>
#include <ks/helper.h>
#include <stdlib.h>
#include <png.h>

void so_img_loader_libpng_load(const char* file, so_img_loader_data_t* info)
{
    png_structp png_ptr;
    png_infop   info_ptr;
    png_bytep*  row_pointers;
    char        abs_file_path[FILENAME_MAX];
    FILE* fp;
    int i;

    ks_helper_path_join_relative_app(abs_file_path, sizeof(abs_file_path), file);
    if (NULL == (fp = fopen(abs_file_path, "rb")))
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

    //png_free_default(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    free(row_pointers);
    fclose(fp);
}
