#include <ks/defs.h>
#include <ks/sys/graphics.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "ks/director.h"
#include "ks/sys/system.h"
#include <stdlib.h>
#include "ks/log.h"

typedef struct graphics_t
{
    ks_extends_sys_graphics();
} graphics_t;

static graphics_t* g = 0;
static GLuint programObject = 0;

static GLuint test_load_shader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint  compiled;

    shader = glCreateShader(type);

    if ( shader == 0 )
        return 0;

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) 
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1)
        {
            char* infoLog = malloc(sizeof(char) * infoLen );

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            ks_log("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static int test_init()
{
    char vShaderStr[] =  
        "attribute vec4 vPosition;    \n"
        "void main()                  \n"
        "{                            \n"
        "   gl_Position = vPosition;  \n"
        "}                            \n";

    char fShaderStr[] =  
        "precision mediump float;                     \n"
        "void main()                                  \n"
        "{                                            \n"
        "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);   \n"
        "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLint  linked;

    vertexShader = test_load_shader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = test_load_shader(GL_FRAGMENT_SHADER, fShaderStr);

    programObject = glCreateProgram();
    if (programObject == 0)
        return 0;

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glBindAttribLocation(programObject, 0, "vPosition");
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked) 
    {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1)
        {
            char* infoLog = malloc(sizeof(char) * infoLen);

            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            ks_log("Error linking program:\n%s\n", infoLog);            

            free(infoLog);
        }

        glDeleteProgram(programObject);
        return GL_FALSE;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return GL_TRUE;
}

static void test_draw()
{
    GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f };

    glViewport(0, 0, ks_director_instance()->width, ks_director_instance()->height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programObject);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void destruct(graphics_t* me)
{
    ks_unused(me);

    ks_container_remove((ks_object_t*)me);
    ks_object_destruct((ks_object_t*)me);
}

static void draw(int x, int y)
{
    test_draw();

    ks_unused(x);
    ks_unused(y);
}

static ks_sys_graphics_interface_t interfaces = {
    0,
    draw
};

KS_API void ks_sys_graphics_init(ks_container_t* container)
{
    g = (graphics_t*)ks_object_new(sizeof(*g));
    g->destruct = (ks_destruct_f)destruct;
    g->klass = &interfaces;

    if (container)
        ks_container_add(container, (ks_object_t*)g);

    test_init();
}

KS_API ks_sys_graphics_t* ks_sys_graphics_instance()
{
    return (ks_sys_graphics_t*)g;
}