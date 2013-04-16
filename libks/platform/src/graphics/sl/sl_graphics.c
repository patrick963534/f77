#include <ks/graphics.h>
#include <ks/director.h>
#include <ks/defs.h>
#include <ks/log.h>
#include <ks/image.h>
#include <ks/system.h>
#include <sl/sl.h>
#include <stdlib.h>
#include "ks/helper.h"

typedef struct graphics_t
{
    ks_extends_graphics();

    char* img_buf;
} graphics_t;

static graphics_t* g;

static void bound_update(int *clip_x, int *clip_y, ks_image_t* img, int *clip_w, int *clip_h ) 
{
    *clip_x = ks_max(0, *clip_x);
    *clip_y = ks_max(0, *clip_y);
    *clip_x = ks_min(img->width,  *clip_x);
    *clip_y = ks_min(img->height, *clip_y);

    *clip_w = ks_min(img->width  - *clip_x, *clip_w);
    *clip_h = ks_min(img->height - *clip_y, *clip_h);
}

static void setup_model(int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h, int img_w, int img_h)
{
    float minx = (float)-clip_w/2;
    float miny = (float)-clip_h/2;
    float maxx = (float) clip_w/2;
    float maxy = (float) clip_h/2;

    float minu = (clip_x) / (float)img_w;
    float minv = (clip_y) / (float)img_h;
    float maxu = (clip_x + clip_w) / (float)img_w;
    float maxv = (clip_y + clip_h) / (float)img_h;

    sl_translate((float)offx, (float)offy, 0);

    sl_begin();
        sl_tex_coord(minu, minv); sl_vertex( minx, maxy, -5.f);
        sl_tex_coord(minu, maxv); sl_vertex( minx, miny, -5.f);
        sl_tex_coord(maxu, maxv); sl_vertex( maxx, miny, -5.f);
 
        sl_tex_coord(minu, minv); sl_vertex( minx, maxy, -5.f);
        sl_tex_coord(maxu, maxv); sl_vertex( maxx, miny, -5.f);
        sl_tex_coord(maxu, minv); sl_vertex( maxx, maxy, -5.f);
    sl_end();
}

static void draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    bound_update(&clip_x, &clip_y, img, &clip_w, &clip_h);

    if (g->img_buf != img->pixels)
    {
        sl_texture_load(img->width, img->height, (unsigned*)img->pixels);
        g->img_buf = img->pixels;
    }

    setup_model(offx, offy, clip_x, clip_y, clip_w, clip_h, img->width, img->height);
}

static void clear_screen()
{
    sl_clear_color(0.5f, 0.5f, 0.5f, 1.0f);
    sl_clear();
}

static void graphics_translate(int x, int y, int z)
{
    sl_translate((float)x, (float)y, (float)z);
}

static void graphics_scale(float sx, float sy)
{
    sl_scale(sx, sy, 1.0f);
}

static void graphics_rotate(float angle)
{
    sl_rotate(angle, 0, 0, 1);
}

static void graphics_push()
{
    sl_push_matrix();
}

static void graphics_pop()
{
    sl_pop_matrix();
}

static void graphics_load_identity()
{
    int width = ks_director_instance()->width;
    int height = ks_director_instance()->height;
    sl_load_identity();
}

static void graphics_flush()
{
    g->buffer = sl_flush();
    ks_system_flush();
}

static void destruct(graphics_t* me)
{
    ks_unused(me);

    ks_log("%s", "destruct gles2_graphics");

    sl_close();
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
        interfaces.scale                = graphics_scale;
        interfaces.rotate               = graphics_rotate;
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
    int width = ks_director_instance()->width;
    int height = ks_director_instance()->height;

    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass    = ks_sys_graphics_interface_instance();
    g->tname    = "r7_graphics";

    if (container)
        ks_object_add(container, (ks_object_t*)g);

    sl_init();
    sl_viewport(0, 0, width, height);

    sl_matrix_mode(sl_matrix_mode_project);
    sl_frustum(0, width * 1.f, height * 1.f, 0, 5.0f, 15.0f);
    sl_matrix_mode(sl_matrix_mode_modelview);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}
