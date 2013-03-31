#include <ks/graphics.h>
#include <ks/image.h>
#include <stdlib.h>

#define call_func(func_, args)  if (func_) func_##args

KS_API void ks_graphics_draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    call_func(ks_graphics_instance()->klass->draw, (img, offx, offy, clip_x, clip_y, clip_w, clip_h));
}

KS_API void ks_graphics_load_identity()
{
    call_func(ks_graphics_instance()->klass->load_identity, ());
}

KS_API void ks_graphics_clear_screen()
{
    call_func(ks_graphics_instance()->klass->clear_screen, ());
}

KS_API void ks_graphics_flush()
{
    call_func(ks_graphics_instance()->klass->flush, ());
}

KS_API void ks_graphics_translate(int x, int y, int z)
{
    call_func(ks_graphics_instance()->klass->translate, (x, y, z));
}

KS_API void ks_graphics_scale(float sx, float sy)
{
    call_func(ks_graphics_instance()->klass->scale, (sx, sy));
}

KS_API void ks_graphics_rotate(float angle)
{
    call_func(ks_graphics_instance()->klass->rotate, (angle));
}

KS_API void ks_graphics_pop()
{
    call_func(ks_graphics_instance()->klass->pop, ());
}

KS_API void ks_graphics_push()
{
    call_func(ks_graphics_instance()->klass->push, ());
}
