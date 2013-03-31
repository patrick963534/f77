#include "img_loader.h"
#include "lodepng/img_loader_lodepng.h"

void so_img_loader_load(const char* file, so_img_loader_data_t* info)
{
    so_img_loader_lodepng_load(file, info);
}