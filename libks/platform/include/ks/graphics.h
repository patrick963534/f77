#ifndef __KS_GRAPHICS_H__
#define __KS_GRAPHICS_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/image.h>
#include <ks/container.h>

typedef struct ks_sys_graphics_interface_t
{
    int     nothing;
    void    (*draw)(ks_image_t* img, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h);
} ks_sys_graphics_interface_t;

#define ks_extends_graphics() \
    ks_extends_object();    \
    ks_sys_graphics_interface_t* klass

typedef struct ks_graphics_t
{
    ks_extends_graphics();
} ks_graphics_t;

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance();

KS_API void ks_graphics_init(ks_container_t* container);
KS_API ks_graphics_t* ks_graphics_instance();

KS_API void ks_graphics_draw(ks_image_t* img, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h);
KS_API void ks_graphics_flush();

#endif
