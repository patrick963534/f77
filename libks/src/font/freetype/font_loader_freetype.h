#ifndef __KS_FONT_LOADER_FREETYPE_H__
#define __KS_FONT_LOADER_FREETYPE_H__

#include <ks/defs.h>
#include "../font_loader.c.h"

void so_font_loader_freetype_load(const char* font_file, int font_size, const char* text, so_font_loader_data_t* info);

#endif