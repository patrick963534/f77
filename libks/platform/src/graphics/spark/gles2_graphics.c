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

    int x = g->pos.x + offx;
    int y = g->pos.y + offy;
    int i = 0;

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
            int dv = *dp;
            int sv = *sp;

            float a = ((sv & 0xFF000000) >> 24) / 255.0f;

            int r = ((dv & 0xFF0000) >> 16) * (1- a) + ((sv & 0xFF0000) >> 16) * a;
            int g = ((dv & 0xFF00)   >> 8 ) * (1- a) + ((sv & 0xFF00)   >>  8) * a;
            int b = ((dv & 0xFF)          ) * (1- a) + ((sv & 0xFF)          ) * a;

            *dp = 0xff000000 | r << 16 | g << 8 | b;

            ++dp;
            ++sp;
        }

        dst += dst_strip;
        src += src_strip;
    }

//    for (i = 0; i < clip_h; ++i)
//    {
//        // Get 167 FPS
//        //memcpy(&dst[(i + y) * dst_strip + x * 4], &src[(i + clip_y) * src_strip + clip_x * 4], clip_w * 4);
//
//        // Get 122 FPS
//        int* dp = &dst[(i + y) * dst_strip + x];
//        int* sp = &src[(i + clip_y) * src_strip + clip_x];
//        int j = 0;
//        while (j++ < clip_w)
//        {
//            int dv = *dp;
//            int sv = *sp;
//            int da = (dv & 0xFF000000) >> 24;
//            int dr = (dv & 0xFF0000) >> 16;
//            int dg = (dv & 0xFF00) >> 8;
//            int db = (dv & 0xFF);
//
//            *dp = sv;
//
//            ++dp;
//            ++sp;
//        }
//
//        // Get 99 FPS only.
////        {
////            int j = 0;
////            for (j = 0; j < clip_w; ++j)
////            {
////                dst[(i + y) * dst_strip + x + j] = src[(i + clip_y) * src_strip + clip_x + j];
////            }
////        }
//    }
}

static void draw2(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    int win_width = ks_director_instance()->width;
    int dst_strip = win_width * 4;
    int src_strip = img->width * 4;

    char* dst = g->buffer;
    char* src = img->pixels;

    int x = g->pos.x + offx;
    int y = g->pos.y + offy;
    int i;

    if (!bounding(&x, &y, &clip_x, &clip_y, &clip_w, &clip_h))
        return;

    for (i = 0; i < clip_h; ++i)
    {
        //memcpy(&dst[(i + y) * dst_strip + x * 4], &src[(i + clip_y) * src_strip + clip_x * 4], clip_w * 4);
         int j;
         for (j = 0; j < clip_w; ++j)
         {
             dst[(i + y) * dst_strip + x * 4 + j * 4 + 0] = src[(i + clip_y) * src_strip + clip_x * 4 + j * 4 + 0];
             dst[(i + y) * dst_strip + x * 4 + j * 4 + 1] = src[(i + clip_y) * src_strip + clip_x * 4 + j * 4 + 1];
             dst[(i + y) * dst_strip + x * 4 + j * 4 + 2] = src[(i + clip_y) * src_strip + clip_x * 4 + j * 4 + 2];
             dst[(i + y) * dst_strip + x * 4 + j * 4 + 3] = src[(i + clip_y) * src_strip + clip_x * 4 + j * 4 + 3];
         }
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
