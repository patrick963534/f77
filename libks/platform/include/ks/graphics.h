#ifndef __KS_GRAPHICS_H__
#define __KS_GRAPHICS_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/image.h>
#include <ks/object.h>

typedef struct ks_sys_graphics_interface_t
{
    int     nothing;
    void    (*draw)(ks_image_t* img, int clip_x, int clip_y, int clip_w, int clip_h);
    void    (*clear_screen)();
    void    (*translate)(int x, int y);
    void    (*pop)();
    void    (*push)();
    void    (*load_identity)();
} ks_sys_graphics_interface_t;

#define ks_extends_graphics() \
    ks_extends_object();    \
    ks_sys_graphics_interface_t* klass

typedef struct ks_graphics_t
{
    ks_extends_graphics();
} ks_graphics_t;

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance();

KS_API void ks_graphics_init(ks_object_t* container);
KS_API ks_graphics_t* ks_graphics_instance();

KS_API void ks_graphics_draw(ks_image_t* img, int clip_x, int clip_y, int clip_w, int clip_h);
KS_API void ks_graphics_translate(int x, int y);
KS_API void ks_graphics_load_identity();
KS_API void ks_graphics_pop();
KS_API void ks_graphics_push();
KS_API void ks_graphics_clear_screen();
KS_API void ks_graphics_flush();

#endif
