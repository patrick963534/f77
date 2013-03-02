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

    char*   buffer;

} graphics_t;

static graphics_t* g = 0;

static void draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    int win_width = ks_director_instance()->width;
    int dst_strip = win_width * 4;
    int src_strip = img->width * 4;

    char* dst = g->buffer;
    char* src = img->pixels;

    int x = g->pos.x + offx;
    int y = g->pos.y + offy;
    int i;

    x = 0;
    y = 0;

    for (i = 0; i < clip_h; ++i) 
    {
        memcpy(&dst[(i + y) * dst_strip + x * 4], &src[(i + clip_y) * src_strip + clip_x * 4], clip_w * 4);
    }
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
    int w = ks_director_instance()->width;
    int h = ks_director_instance()->height;

    char* pBits = (char*)ks_system_instance()->ptr;
    char* src = g->buffer;

    int strip = w * 4 + (4 - w * 3 / 4) % 4;
    int i;

    for (i = 0; i < h; ++i) 
    {
        memcpy(&pBits[i * strip], &src[i * w * 4], w * 4);
    }

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
    ks_director_t* d = ks_director_instance();

    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass    = ks_sys_graphics_interface_instance();
    g->tname    = "gles2_graphics";
    g->top      = -1;
    g->buffer   = calloc(1, d->width * d->height * 4);

    if (container)
        ks_object_add(container, (ks_object_t*)g);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}
