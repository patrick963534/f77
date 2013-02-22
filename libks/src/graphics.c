#include <ks/graphics.h>
#include <ks/image.h>
#include <stdlib.h>

KS_API void ks_graphics_draw(ks_image_t* img, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h)
{
    ks_graphics_instance()->klass->draw(img, x, y, clip_x, clip_y, clip_w, clip_h);
}

KS_API void ks_graphics_flush()
{
    
}