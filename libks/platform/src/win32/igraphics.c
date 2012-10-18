#include <ks/defs.h>
#include <ks/sys/graphics.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "ks/director.h"
#include "ks/sys/system.h"
#include <stdlib.h>
#include "ks/log.h"

static void draw(int x, int y)
{
    ks_unused(x);
    ks_unused(y);
}

static ks_sys_graphics_interface_t interfaces = {
    0,
    draw
};

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance()
{
    return &interfaces;
}