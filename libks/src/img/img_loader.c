#include "img_loader.h"
#include "libpng/img_loader_libpng.h"

char* ks_img_loader_load(const char* file, int* w, int* h)
{
    return ks_img_loader_libpng_load(file, w, h);
}