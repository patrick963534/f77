#include "font_loader.h"
#include "freetype/font_loader_freetype.h"

void so_font_loader_load(const char* file, so_font_loader_data_t* info)
{
    so_font_loader_freetype_load(file, info);
}