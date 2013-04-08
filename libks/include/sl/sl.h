#ifndef __SKY_LIBRARY_H__
#define __SKY_LIBRARY_H__

#define SL_API      extern

typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned char   uchar;

typedef enum sl_matrix_mode_e
{
    sl_matrix_mode_project,
    sl_matrix_mode_model,
} sl_matrix_mode_e;

typedef enum sl_cull_face_mode_e
{
    sl_cull_face_mode_font,
    sl_cull_face_mode_back,
} sl_cull_face_mode_e;

typedef enum sl_mode_e
{
    sl_mode_cull_face,
} sl_mode_e;

SL_API void     sl_init();
SL_API void     sl_flush(uint* target, int w, int h);
SL_API void     sl_clear_color(float r, float g, float b, float a);
SL_API void     sl_clear();

SL_API void     sl_enable(sl_mode_e mode);
SL_API void     sl_disable(sl_mode_e mode);

SL_API void     sl_viewport(int x, int y, int w, int h);
SL_API void     sl_frustum(float left, float right, float bottom, float top, float near, float far);

SL_API void     sl_matrix_mode(sl_matrix_mode_e mode);
SL_API void     sl_load_identity();
SL_API void     sl_translate(float x, float y, float z);
SL_API void     sl_rotate(float angle, float x, float y, float z);
SL_API void     sl_scale(float x, float y, float z);
SL_API void     sl_push_matrix();
SL_API void     sl_pop_matrix();

SL_API void     sl_cull_face(sl_cull_face_mode_e mode);
SL_API void     sl_begin();
SL_API void     sl_vertex(float x, float y, float z);
SL_API void     sl_tex_coord(float u, float v);
SL_API void     sl_end();

#endif