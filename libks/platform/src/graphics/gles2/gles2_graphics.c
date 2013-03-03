#include <ks/graphics.h>
#include <ks/director.h>
#include <ks/defs.h>
#include <ks/log.h>
#include <ks/image.h>
#include <ks/system.h>
#include <GLES2/gl2.h>
#include <stdlib.h>

#include "gles2_shader.h"

#define Pos_Stack_Size  256

// Maybe it's better to use 'Renderer' for different 'program'.
typedef struct tex_program_t
{
    GLuint      program;

    GLuint      texture_id;
    GLint       position_loc;
    GLint       texCoord_loc;
    GLint       sample_loc;
    GLint       mvp_loc;
    const char* img_file;

} tex_program_t;

typedef struct draw_pos_t
{
    int x;
    int y;
    int z;
} draw_pos_t;

typedef struct graphics_t
{
    ks_extends_graphics();

    tex_program_t tex_render;

    draw_pos_t  pos;
    draw_pos_t  pos_stack[Pos_Stack_Size];
    int top;
} graphics_t;

#define Vertex_Max_Count    2048

static graphics_t* g = 0;
static GLushort indices[Vertex_Max_Count * 6];
static GLfloat vecCoords[Vertex_Max_Count * 12];
static GLfloat texCoords[Vertex_Max_Count * 8];

static int verCoords_count = 0;
static int texCoords_count = 0;
static int indices_count = 0;

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

static void generate_vec_coords(GLfloat* p, int offx, int offy, int clip_w, int clip_h)
{
    int x = g->pos.x + offx;
    int y = g->pos.y + offy;
    GLfloat z = (GLfloat)g->pos.z;
    int all_w = ks_director_instance()->width;
    int all_h = ks_director_instance()->height;
    float factor = 2.0f; // The default value is 2.0f. Value 0.5f means scale down 4 times.

    float minX = x * factor / (float)all_w - 1.0f;
    float minY = y * factor / (float)all_h - 1.0f;
    float maxX = minX + clip_w * factor / (float)all_w;
    float maxY = minY + clip_h * factor / (float)all_h;

    p[0] = minX; p[1]  = maxY; p[2]  = z;
    p[3] = minX; p[4]  = minY; p[5]  = z;
    p[6] = maxX; p[7]  = minY; p[8]  = z;
    p[9] = maxX; p[10] = maxY; p[11] = z;
}

static void generate_tex_coords(GLfloat* p, int clip_x, int clip_y, int clip_w, int clip_h, int all_w, int all_h)
{
    float minX = (clip_x) / (float)all_w;
    float minY = (clip_y) / (float)all_h;
    float maxX = (clip_x + clip_w) / (float)all_w;
    float maxY = (clip_y + clip_h) / (float)all_h;

    p[0] = minX; p[1] = minY;
    p[2] = minX; p[3] = maxY;
    p[4] = maxX; p[5] = maxY;
    p[6] = maxX; p[7] = minY;
}

static void setup_model(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    static GLushort base_indices[] = {0, 1, 2, 0, 2, 3};
    GLushort base = (GLushort)(indices_count * 4 / 6);
    int i = 0;

    clip_x = ks_max(0, clip_x);
    clip_y = ks_max(0, clip_y);
    clip_x = ks_min(img->width,  clip_x);
    clip_y = ks_min(img->height, clip_y);

    clip_w = ks_min(img->width  - clip_x, clip_w);
    clip_h = ks_min(img->height - clip_y, clip_h);

    generate_vec_coords(&vecCoords[verCoords_count], offx, offy, clip_w, clip_h);
    generate_tex_coords(&texCoords[texCoords_count], clip_x, clip_y, clip_w, clip_h, img->width, img->height);

    for (i = 0; i < 6; i++)
    {
        indices[indices_count + i] = base_indices[i] + base;
    }

    verCoords_count += 12;
    texCoords_count += 8;
    indices_count += 6;
}

static void real_draw()
{
    if (indices_count == 0)
        return;

    glVertexAttribPointer(g->tex_render.position_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vecCoords);
    glVertexAttribPointer(g->tex_render.texCoord_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), texCoords);

    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_SHORT, indices);

    verCoords_count = 0;
    texCoords_count = 0;
    indices_count = 0;
}

static void draw(ks_image_t* img, int offx, int offy, int clip_x, int clip_y, int clip_w, int clip_h)
{
    if (verCoords_count == Vertex_Max_Count * 6)
        real_draw();

    setup_model(img, offx, offy, clip_x, clip_y, clip_w, clip_h);

    if (g->tex_render.img_file != img->file)
    {
        if (g->tex_render.texture_id != 0)
        {
            real_draw();
            glDeleteTextures(1, &g->tex_render.texture_id);
        }

        g->tex_render.texture_id = CreateTexture2D(img);
        g->tex_render.img_file = img->file;

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g->tex_render.texture_id);
        glUniform1i(g->tex_render.sample_loc, 0);

        ks_log("create texture");
    }
}

static void clear_screen()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
}

static void graphics_flush()
{
    real_draw();
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
    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass    = ks_sys_graphics_interface_instance();
    g->tname    = "gles2_graphics";
    g->top      = -1;

    if (container)
        ks_object_add(container, (ks_object_t*)g);

    glViewport(0, 0, ks_director_instance()->width, ks_director_instance()->height);

    g->tex_render.program = ks_gles2_shader_program_for(ks_gles2_program_type_texture);
    g->tex_render.position_loc = glGetAttribLocation(g->tex_render.program, "a_position");
    g->tex_render.texCoord_loc = glGetAttribLocation(g->tex_render.program, "a_texCoord");
    g->tex_render.sample_loc = glGetUniformLocation(g->tex_render.program, "s_texture");
    g->tex_render.mvp_loc = glGetUniformLocation(g->tex_render.program, "m_mvp");
    glUseProgram(g->tex_render.program);

    glEnableVertexAttribArray(g->tex_render.position_loc);
    glEnableVertexAttribArray(g->tex_render.texCoord_loc);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}
