#include <ks/graphics.h>
#include <ks/director.h>
#include <ks/defs.h>
#include <ks/log.h>
#include <ks/image.h>
#include <ks/system.h>
#include <stdlib.h>
#include <string.h>

#define Pos_Stack_Size  256

typedef struct draw_pos_t
{
    int x;
    int y;
    int z;
} draw_pos_t;

typedef struct graphics_t
{
    ks_extends_graphics();

    draw_pos_t  pos;
    draw_pos_t  pos_stack[Pos_Stack_Size];
    int top;

} graphics_t;

static graphics_t* g = 0;

static void draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    char* pBits = (char*)ks_system_instance()->ptr;
    char* src = img->pixels;
    int mainW = ks_director_instance()->width;
    int w = img->width;
    int h = img->height;
    int align = (4 - w * 3 / 4) % 4;
    int strip = mainW * 4 + align;
    int i;

    for (i = 0; i < h; ++i) 
    {
        memcpy(&pBits[i * strip], &src[i * w * 4], w * 4);
    }

    ks_unused(img);
    ks_unused(offx);
    ks_unused(offy);
    ks_unused(clip_x);
    ks_unused(clip_y);
    ks_unused(clip_w);
    ks_unused(clip_h);
}

static void clear_screen()
{
    
}

static void graphics_translate(int x, int y, int z)
{
    g->pos.x += x;
    g->pos.y += y;
    g->pos.z += z;
}

static void graphics_push()
{
    if (g->top >= Pos_Stack_Size - 1)
        ks_assert(0, "The graphics pos stack already is full.");

    g->pos_stack[++g->top] = g->pos;
}

static void graphics_pop()
{
    if (g->top < 0)
        ks_assert(0, "The graphics pos stack already be emtpy.");

    g->pos = g->pos_stack[g->top--];
}

static void graphics_load_identity()
{
    g->pos.x = 0;
    g->pos.y = 0;
    g->pos.z = 0;
}

static void graphics_flush()
{
    ks_system_flush();
}

static void destruct(graphics_t* me)
{
    ks_log("%s", "destruct gles2_graphics");

    ks_object_destruct((ks_object_t*)me);
}

static ks_sys_graphics_interface_t interfaces = {
    0,
    draw,
    clear_screen,
    graphics_translate,
    graphics_pop,
    graphics_push,
    graphics_load_identity,
    graphics_flush,
};

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance()
{
    return &interfaces;
}

KS_API void ks_graphics_init(ks_object_t* container)
{
    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass    = ks_sys_graphics_interface_instance();
    g->tname    = "gles2_graphics";
    g->top      = -1;

    if (container)
        ks_object_add(container, (ks_object_t*)g);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}
