#include "img_loader.h"
#include "libpng/img_loader_libpng.h"

void so_img_loader_load(const char* file, so_img_loader_data_t* info)
{
    return so_img_loader_libpng_load(file, info);
}