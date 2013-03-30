#include <stdlib.h>
#include "r7_zbuffer.h"

void ZB_setTexture(ZBuffer *zb, unsigned short *texture, int w, int h)
{
    zb->current_texture=texture;
    zb->tex_w = w;
    zb->tex_h = h;
}