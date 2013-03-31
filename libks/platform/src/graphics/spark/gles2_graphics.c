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

static int bounding(int* x, int* y, int* cx, int* cy, int* cw, int* ch)
{
    int win_width = ks_director_instance()->width;
    int win_height = ks_director_instance()->height;

    int minx = *x;
    int miny = *y;
    int maxx = minx + *cw;
    int maxy = miny + *ch;

    if (minx >= win_width || maxx <= 0 || miny >= win_height || maxy <= 0)
        return 0;

    if (minx < 0)
        minx = 0;

    if (maxx > win_width)
        maxx = win_width;

    if (miny < 0)
        miny = 0;

    if (maxy > win_height)
        maxy = win_height;

    if (minx != *x)
        *cx += (minx - *x);

    if (miny != *y)
        *cy += (miny - *y);

    *x = minx;
    *y = miny;
    *cw = maxx - minx;
    *ch = maxy - miny;

    if (*cw == 0 || *ch == 0)
        return 0;

    return 1;
}

static void draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    int dst_strip = ks_director_instance()->width;
    int src_strip = img->width;

    int* dst = (int*)g->buffer;
    int* src = (int*)img->pixels;

    int x = g->pos.x + offx - clip_w / 2;
    int y = ks_director_instance()->height - (g->pos.y + offy + clip_h / 2);
    int i = 0;

    if (img->width == 0 || img->height == 0 || clip_w == 0 || clip_h == 0)
        return;

    if (!bounding(&x, &y, &clip_x, &clip_y, &clip_w, &clip_h))
        return;

    dst = dst + y * dst_strip + x;
    src = src + clip_y * src_strip + clip_x;

    while (i++ < clip_h)
    {
        int* dp = dst;
        int* sp = src;
        int j = 0;

        while (j++ < clip_w)
        {
            int sv = *sp;
            int sa = ((sv & 0xFF000000) >> 24);

            if (sa == 0xff)
            {
                *dp = sv;
            }
            else if (sa != 0)
            {
                int dv = *dp;
                float a = sa / 255.0f;

                int r = (int)(((dv >> 16) & 0xFF) * (1- a) + ((sv >> 16) & 0xFF) * a);
                int g = (int)(((dv >>  8) & 0xFF) * (1- a) + ((sv >>  8) & 0xFF) * a);
                int b = (int)(((dv      ) & 0xFF) * (1- a) + ((sv      ) & 0xFF) * a);

                *dp = 0xff000000 | r << 16 | g << 8 | b;
            }

            ++dp;
            ++sp;
        }

        dst += dst_strip;
        src += src_strip;
    }
}

static void clear_screen()
{
    int w = ks_director_instance()->width;
    int h = ks_director_instance()->height;
    memset(g->buffer, 0, w * h * 4);
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

static ks_sys_graphics_interface_t interfaces;

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance()
{
    static int init_interface = 1;

    if (init_interface)
    {
        interfaces.draw                 = draw;
        interfaces.clear_screen         = clear_screen;
        interfaces.translate            = graphics_translate;
        interfaces.pop                  = graphics_pop;
        interfaces.push                 = graphics_push;
        interfaces.load_identity        = graphics_load_identity;
        interfaces.flush                = graphics_flush;

        init_interface = 0;
    }

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
