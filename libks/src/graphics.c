#include <ks/graphics.h>
#include <ks/image.h>
#include <ks/system.h>
#include <stdlib.h>

KS_API void ks_graphics_draw(ks_image_t* img, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h)
{
    ks_graphics_instance()->klass->draw(img, x, y, clip_x, clip_y, clip_w, clip_h);
}

KS_API void ks_graphics_clear_screen()
{
    ks_graphics_instance()->klass->clear_screen();
}

KS_API void ks_graphics_flush()
{
    ks_system_flush();
}

KS_API void ks_graphics_translate(int x, int y)
{
    ks_graphics_instance()->klass->translate(x, y);
}

KS_API void ks_graphics_pop()
{
    ks_graphics_instance()->klass->pop();
}

KS_API void ks_graphics_push()
{
    ks_graphics_instance()->klass->push();
}
