#include <stdlib.h>
#include "r7_zbuffer.h"

void ZB_setTexture(ZBuffer *zb,PIXEL *texture)
{
    zb->current_texture=texture;
}