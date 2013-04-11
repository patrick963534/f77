#include "sl.c.h"
#include <sl/sl.h>

SL_API void sl_texture_load(int w, int h, uint* pixels)
{
    sl_context_t* c = sl_context();

    c->zbuffer.tex_w = w;
    c->zbuffer.tex_h = h;
    c->zbuffer.texture = pixels;
}

