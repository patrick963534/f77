#include "sl.c.h"
#include <sl/sl.h>

SL_API void sl_viewport(int x, int y, int w, int h)
{
    sl_context_t* c = sl_context();

    if (w <= 0 || h <= 0 || (x + w) <= 0 || (y + h) <= 0)
        sl_error_exit("wrong parameter values.");

    if (c->viewport.x != x || c->viewport.y != y || c->viewport.w != w || c->viewport.h != h) 
    {
        c->viewport.x = x;
        c->viewport.y = y;
        c->viewport.w = w;
        c->viewport.h = h;
        c->viewport.updated = 1;

        free(c->zbuffer.pbuf);
        free(c->zbuffer.zbuf);
        c->zbuffer.pbuf = sl_calloc(1, w * h * sizeof(c->zbuffer.pbuf[0]));
        c->zbuffer.zbuf = sl_calloc(1, w * h * sizeof(c->zbuffer.zbuf[0]));
    }
}