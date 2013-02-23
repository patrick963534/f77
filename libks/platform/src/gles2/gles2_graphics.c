#include <ks/graphics.h>
#include <ks/director.h>
#include <ks/defs.h>
#include <ks/log.h>
#include <ks/image.h>

#include <GLES2/gl2.h>
#include <stdlib.h>

#include "gles2_shader.h"

// Maybe it's better to use 'Renderer' for different 'program'.
typedef struct tex_program_t
{
    GLuint      program;

    GLuint      texture_id;
    GLint       positionLoc;
    GLint       texCoordLoc;
    GLint       samplerLoc;
    ks_image_t* img;

} tex_program_t;

typedef struct graphics_t
{
    ks_extends_graphics();

    tex_program_t tex_render;
} graphics_t;

static graphics_t* g = 0;
static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
static GLfloat vecCoords[12];
static GLfloat texCoords[8];

static GLuint CreateTexture2D(ks_image_t* img)
{
    GLuint textureId;

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;
}

static void generate_vec_coords(GLfloat* vecCoords, int x, int y, int clip_w, int clip_h)
{
    int all_w = ks_director_instance()->width;
    int all_h = ks_director_instance()->height;
    int org_offx = -all_w;
    int org_offy = -all_h;

    float factor = 1.0f;
    float minX = (x + org_offx) / (float)all_w;
    float minY = (y + org_offy) / (float)all_h;
    float maxX = (x + clip_w + org_offx) / (float)all_w;
    float maxY = (y + clip_h + org_offy) / (float)all_h;

    vecCoords[0] = minX * factor;
    vecCoords[1] = maxY * factor;
    vecCoords[2] = 0;

    vecCoords[3] = minX * factor;
    vecCoords[4] = minY * factor;
    vecCoords[5] = 0;

    vecCoords[6] = maxX * factor;
    vecCoords[7] = minY * factor;
    vecCoords[8] = 0;

    vecCoords[9]  = maxX * factor;
    vecCoords[10] = maxY * factor;
    vecCoords[11] = 0;
}

static void generate_tex_coords(GLfloat* texCoords, int clip_x, int clip_y, int clip_w, int clip_h, int all_w, int all_h)
{
    float minX = (clip_x) / (float)all_w;
    float minY = (clip_y) / (float)all_h;
    float maxX = (clip_x + clip_w) / (float)all_w;
    float maxY = (clip_y + clip_h) / (float)all_h;

    texCoords[0] = minX;
    texCoords[1] = minY;

    texCoords[2] = minX;
    texCoords[3] = maxY;

    texCoords[4] = maxX;
    texCoords[5] = maxY;

    texCoords[6] = maxX;
    texCoords[7] = minY;
}

static void setup_model(ks_image_t* img, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h)
{
    clip_x = ks_max(0, clip_x);
    clip_y = ks_max(0, clip_y);
    clip_x = ks_min(img->width,  clip_x);
    clip_y = ks_min(img->height, clip_y);

    clip_w = ks_min(img->width  - clip_x, clip_w);
    clip_h = ks_min(img->height - clip_y, clip_h);

    generate_vec_coords(vecCoords, x, y, clip_w, clip_h);
    generate_tex_coords(texCoords, clip_x, clip_y, clip_w, clip_h, img->width, img->height);

    glVertexAttribPointer(g->tex_render.positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vecCoords);
    glVertexAttribPointer(g->tex_render.texCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), texCoords);

    glEnableVertexAttribArray(g->tex_render.positionLoc);
    glEnableVertexAttribArray(g->tex_render.texCoordLoc);
}

static void draw(ks_image_t* img, int x, int y, int clip_x, int clip_y, int clip_w, int clip_h)
{
    setup_model(img, x, y, clip_x, clip_y, clip_w, clip_h);

    if (g->tex_render.img != img)
    {
        if (g->tex_render.texture_id != 0)
            glDeleteTextures(1, &g->tex_render.texture_id);

        g->tex_render.texture_id = CreateTexture2D(img);
        g->tex_render.img = img;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g->tex_render.texture_id);
    glUniform1i(g->tex_render.samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

static void clear_screen()
{
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
};

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance()
{
    return &interfaces;
}

KS_API void ks_graphics_init(ks_object_t* container)
{
    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass = ks_sys_graphics_interface_instance();
    g->tname = "gles2_graphics";

    if (container)
        ks_object_add(container, (ks_object_t*)g);



    glViewport(0, 0, ks_director_instance()->width, ks_director_instance()->height);

    g->tex_render.program = ks_gles2_shader_program_for(ks_gles2_program_type_texture);
    g->tex_render.positionLoc = glGetAttribLocation(g->tex_render.program, "a_position");
    g->tex_render.texCoordLoc = glGetAttribLocation(g->tex_render.program, "a_texCoord");
    g->tex_render.samplerLoc = glGetUniformLocation(g->tex_render.program, "s_texture");
    glUseProgram(g->tex_render.program);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}
