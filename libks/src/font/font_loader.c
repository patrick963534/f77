#include "font_loader.h"
#include "freetype/font_loader_freetype.h"

void so_font_loader_load(const char* font_file, int font_size, const char* text, so_font_loader_data_t* info)
{
    so_font_loader_freetype_load(font_file, font_size, text, info);
}