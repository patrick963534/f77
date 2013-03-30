#include <ks/graphics.h>
#include <ks/director.h>
#include <ks/defs.h>
#include <ks/log.h>
#include <ks/image.h>
#include <ks/system.h>
#include <r7/gl.h>
#include <stdlib.h>
#include "ks/helper.h"

typedef struct graphics_t
{
    ks_extends_graphics();

    char* img_buf;

    GLuint textureId; 
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
    int x = offx;
    int y = offy;
    int all_w = ks_director_instance()->width;
    int all_h = ks_director_instance()->height;

    float minx = x - all_w / 2;
    float miny = y - all_h / 2;
    float maxx = minx + clip_w;
    float maxy = miny + clip_h;

    float minu = (clip_x) / (float)img_w;
    float minv = (clip_y) / (float)img_h;
    float maxu = (clip_x + clip_w) / (float)img_w;
    float maxv = (clip_y + clip_h) / (float)img_h;

    glBegin(GL_TRIANGLES);
        glTexCoord2f(minu, minv); glVertex3f( minx, maxy, -5.f);
        glTexCoord2f(minu, maxv); glVertex3f( minx, miny, -5.f);
        glTexCoord2f(maxu, maxv); glVertex3f( maxx, miny, -5.f);
 
        glTexCoord2f(minu, minv); glVertex3f( minx, maxy, -5.f);
        glTexCoord2f(maxu, maxv); glVertex3f( maxx, miny, -5.f);
        glTexCoord2f(maxu, minv); glVertex3f( maxx, maxy, -5.f);
    glEnd();
}

static void draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    offx -= clip_w / 2;
    offy -= clip_h / 2;

    bound_update(&clip_x, &clip_y, img, &clip_w, &clip_h);

    if (g->img_buf != img->pixels)
    {
        if (g->img_buf != NULL)
            glDeleteTextures(1, &g->textureId);

        // Use tightly packed data
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &g->textureId);
        glBindTexture(GL_TEXTURE_2D, g->textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, g->textureId);
        g->img_buf = img->pixels;
    }

    setup_model(offx, offy, clip_x, clip_y, clip_w, clip_h, img->width, img->height);
}

static void clear_screen()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void graphics_translate(int x, int y, int z)
{
    glTranslatef((float)x, (float)y, (float)z);
}

static void graphics_push()
{
    glPushMatrix();
}

static void graphics_pop()
{
    glPopMatrix();
}

static void graphics_load_identity()
{
    glLoadIdentity();
}

static void graphics_flush()
{
    g->buffer = glFlush();
    ks_helper_image565_save_ppm("flush.ppm", g->buffer, ks_director_instance()->width, ks_director_instance()->height);
    ks_system_flush();
}

static void destruct(graphics_t* me)
{
    ks_unused(me);

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
    int width = ks_director_instance()->width;
    int height = ks_director_instance()->height;

    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass    = ks_sys_graphics_interface_instance();
    g->tname    = "r7_graphics";

    if (container)
        ks_object_add(container, (ks_object_t*)g);

    glInit();
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-width/2, width/2, -height/2, height/2, 5.0f, 15.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}
