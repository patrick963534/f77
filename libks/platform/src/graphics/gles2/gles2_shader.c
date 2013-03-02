#include "gles2_shader.h"
#include <ks/log.h>
#include <stdlib.h>

typedef struct program_t
{
    const char* vs;
    const char* fs;
} program_t;

static program_t tex_program = {
    "attribute vec4 a_position;   \n"
    "attribute vec2 a_texCoord;   \n"
    "uniform mat4 m_mvp;          \n"
    "varying vec2 v_texCoord;     \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = a_position; \n"
    "   v_texCoord = a_texCoord;  \n"
    "}                            \n",

    "precision mediump float;                            \n"
    "varying vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "void main()                                         \n"
    "{                                                   \n"
    "  gl_FragColor = texture2D(s_texture, v_texCoord);  \n"
    "}                                                   \n",
};

static GLuint load_shader(GLenum type, const char *shaderSrc)
{
    GLuint shader = 0;
    GLint compiled;

    if (0 == (shader = glCreateShader(type)))
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
            char* infoLog = malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog);
            ks_log("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint ks_gles2_shader_program_from(const char* vs_str, const char* fs_str)
{
    GLuint vs = 0;
    GLuint fs = 0;
    GLuint p  = 0;
    GLint linked;

    if (0 == (vs = load_shader(GL_VERTEX_SHADER, vs_str)))
        goto fail;

    if (0 == (fs = load_shader(GL_FRAGMENT_SHADER, fs_str)))
        goto fail;

    if (0 == (p = glCreateProgram()))
        goto fail;

    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);

    glDeleteShader(vs);
    glDeleteShader(fs);
    vs = 0;
    fs = 0;

    // Check the link status
    glGetProgramiv(p, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = malloc(infoLen);
            glGetProgramInfoLog(p, infoLen, NULL, infoLog);
            ks_log("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(p);
        return 0;
    }

    return p;

fail:
    ks_log("%s", "fail to load program or shader");

    if (vs != 0)
        glDeleteShader(vs);
    if (fs != 0)
        glDeleteShader(fs);

    return 0;
}

GLuint ks_gles2_shader_program_for(ks_gles2_program_type_e type)
{
    switch (type)
    {
    case ks_gles2_program_type_texture:
        return ks_gles2_shader_program_from(tex_program.vs, tex_program.fs);
    default:
        return 0;
    }
}
