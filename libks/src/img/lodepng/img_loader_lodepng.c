#include "img_loader_lodepng.h"
#include "lodepng.h"
#include <ks/log.h>
#include <ks/helper.h>
#include <stdlib.h>
#include <png.h>

void so_img_loader_lodepng_load(const char* file, so_img_loader_data_t* info)
{
    unsigned error;
    unsigned char* image;
    unsigned width, height;
    unsigned char* png;
    size_t pngsize;
    char   abs_file_path[FILENAME_MAX];

    ks_helper_path_join_relative_app(abs_file_path, sizeof(abs_file_path), file);

    lodepng_load_file(&png, &pngsize, abs_file_path);
    error = lodepng_decode32(&image, &width, &height, png, pngsize);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    free(png);

    info->pixels = (char*)image;
    info->width = width;
    info->height = height;
}
