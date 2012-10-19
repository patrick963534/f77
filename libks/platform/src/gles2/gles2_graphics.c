#include <ks/graphics.h>
#include <ks/director.h>
#include <ks/defs.h>
#include <ks/log.h>

#include <GLES2/gl2.h>
#include <stdlib.h>

#include "gles2_shader.h"

typedef struct tex_program_t
{
    GLuint      program;

    GLuint      texture_id;
    GLint       positionLoc;
    GLint       texCoordLoc;
    GLint       samplerLoc;

} tex_program_t;

typedef struct graphics_t
{
    ks_extends_graphics();

    tex_program_t tex_render;
} graphics_t;

static graphics_t* g = 0;
static GLuint programObject = 0;

static GLuint CreateSimpleTexture2D()
{
    GLuint textureId;

    GLubyte pixels[4 * 3] = { 255,   0,   0,  // Red
                              0, 255,   0,    // Green
                              0,   0, 255,    // Blue
                              255, 255,   0   // Yellow
    };

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;
}

static void draw(int x, int y)
{
    GLfloat vVertices[] = {-0.5f,  0.5f, 0.0f, // Position 0
                            0.0f,  0.0f,       // TexCoord 0 
                           -0.5f, -0.5f, 0.0f, // Position 1
                            0.0f,  1.0f,       // TexCoord 1
                            0.5f, -0.5f, 0.0f, // Position 2
                            1.0f,  1.0f,       // TexCoord 2
                            0.5f,  0.5f, 0.0f, // Position 3
                            1.0f,  0.0f        // TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    if (g->tex_render.texture_id == 0)
        g->tex_render.texture_id = CreateSimpleTexture2D();

    glViewport(0, 0, ks_director_instance()->width, ks_director_instance()->height);
    glClear(GL_COLOR_BUFFER_BIT);

    if (g->tex_render.program == 0)
    {
        g->tex_render.program = ks_gles2_shader_program_for(ks_gles2_program_type_texture);
        g->tex_render.positionLoc = glGetAttribLocation(g->tex_render.program, "a_position");
        g->tex_render.texCoordLoc = glGetAttribLocation(g->tex_render.program, "a_texCoord");
        g->tex_render.samplerLoc = glGetUniformLocation(g->tex_render.program, "s_texture");
    }

    glUseProgram(g->tex_render.program);

    glVertexAttribPointer(g->tex_render.positionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    glVertexAttribPointer(g->tex_render.texCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

    glEnableVertexAttribArray(g->tex_render.positionLoc);
    glEnableVertexAttribArray(g->tex_render.texCoordLoc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g->tex_render.texture_id);
    glUniform1i(g->tex_render.samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    ks_unused(x);
    ks_unused(y);
    //test_draw();
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
};

ks_sys_graphics_interface_t* ks_sys_graphics_interface_instance()
{
    return &interfaces;
}

KS_API void ks_graphics_init(ks_container_t* container)
{
    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass = ks_sys_graphics_interface_instance();
    g->tname = "gles2_graphics";

    if (container)
        ks_container_add(container, (ks_object_t*)g);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

KS_API ks_graphics_t* ks_graphics_instance()
{
    return (ks_graphics_t*)g;
}