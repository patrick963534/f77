#ifndef __KS_GLES2_SHADER_H__
#define __KS_GLES2_SHADER_H__

#include <GLES2/gl2.h>

typedef enum ks_gles2_program_type_e
{
    ks_gles2_program_type_texture,
} ks_gles2_program_type_e;

GLuint ks_gles2_shader_program_from(const char* vs, const char* fs);
GLuint ks_gles2_shader_program_for(ks_gles2_program_type_e type);

#endif