#include "sl.c.h"
#include <stdlib.h>

static void memset_l(void *adr, int val, int count)
{
    int i, n, v;
    unsigned int *p;

    p = adr;
    v = val;
    n = count >> 2;
    for (i = 0; i < n; i++) 
    {
        p[0] = v;
        p[1] = v;
        p[2] = v;
        p[3] = v;
        p += 4;
    }

    n = count & 3;
    for (i = 0; i < n; i++)
        *p++ = val;
}

SL_API void sl_clear_color(float r, float g, float b, float a)
{
    sl_context_t* c = sl_context();
    uint ir = min(255, (uint)(r * 255));
    uint ig = min(255, (uint)(g * 255));
    uint ib = min(255, (uint)(b * 255));
    uint ia = min(255, (uint)(a * 255));

    c->clear_color = (ia << 24) | (ir << 16) | (ig << 8) | ib;
}

SL_API void sl_clear()
{
    sl_context_t* c = sl_context();
    int color = c->clear_color;

    memset_l(c->zbuffer.pbuf, color, c->zbuffer.w * c->zbuffer.h);
}