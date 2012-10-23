#include <ks/graphics.h>
#include <ks/image.h>
#include <stdlib.h>

KS_API void ks_graphics_draw(ks_image_t* img, int x, int y, int w, int h)
{
    ks_graphics_instance()->klass->draw(img, x, y, w, h);
}

KS_API void ks_graphics_flush()
{
    
}