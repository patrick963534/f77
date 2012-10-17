#ifndef __KS_SYS_GRAPHICS_H__
#define __KS_SYS_GRAPHICS_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/container.h>

typedef struct ks_sys_graphics_interface_t
{
    int     nothing;
    void    (*draw)(int x, int y);
} ks_sys_graphics_interface_t;

#define ks_extends_sys_graphics() \
    ks_extends_object();    \
    ks_sys_graphics_interface_t* klass

typedef struct ks_sys_graphics_t
{
    ks_extends_sys_graphics();
} ks_sys_graphics_t;

KS_API void               ks_sys_graphics_init(ks_container_t* container);
KS_API ks_sys_graphics_t* ks_sys_graphics_instance();

#endif
