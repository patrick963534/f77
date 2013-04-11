#ifndef __SKY_LIBRARY_C_H__
#define __SKY_LIBRARY_C_H__

#include <sl/sl.h>
#include "sl_matrix.h"

#define SL_PI		        3.14159265358979323846
#define NULL                ((void*)0)
#define sl_unused(a)        (void)(a)

#define Matrix_Stack_Size   32

typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned char   uchar;

typedef struct sl_zbuffer_t
{
    int     w, h;
    int     pitch;

    ushort* zbuf;
    uint*   pbuf;

    uint*   texture;
    int     tex_w;
    int     tex_h;
} sl_zbuffer_t;

typedef struct sl_viewport_t
{
    int         x, y, w, h;
    sl_vector_t scale, trans;
    int         updated;
} sl_viewport_t;

typedef struct sl_zb_point_t
{
    int     x, y, z;
    float   u, v;
    int     r, g, b;
} sl_zb_point_t;

typedef struct sl_vertex_t
{
    sl_vector_t     pos;
    float           u, v;

    sl_vector_t     eye;
    sl_vector_t     prj;
    sl_zb_point_t   zp;
} sl_vertex_t;

typedef struct sl_context_t
{
    sl_matrix_t             matrix_model_view_projection;
    sl_matrix_t             matrix[3];
    sl_matrix_t*            matrix_projection;
    sl_matrix_t*            matrix_model_view;
    sl_matrix_t*            matrix_texture;
    sl_matrix_t*            matrix_current;
    int                     matrix_mode;
    
    sl_matrix_t             matrix_stacks[3][Matrix_Stack_Size];
    sl_matrix_t*            matrix_stacks_ptr[3];

    int                     is_cull_face;
    sl_cull_face_mode_e     cull_face_mode;

    sl_viewport_t           viewport;
    sl_zbuffer_t            zbuffer;

    float                   u, v;
    sl_vertex_t             vertexs[3];
    int                     nvertex;

    int                     begin;
} sl_context_t;

void*         sl_malloc(int sz);
void*         sl_calloc(int count, int sz);
void          sl_free(void* p);

void          sl_error_exit(const char* format, ...);
sl_context_t* sl_context();
int           sl_normalize(float* x, float* y, float* z);

void          sl_vector_mul_matrix(sl_vector_t* c, const sl_vector_t* left, const sl_matrix_t* right);
void          sl_transform_viewport(sl_vertex_t* v);
void          sl_clip_draw();
void          sl_rasterize(sl_zb_point_t* p0, sl_zb_point_t* p1, sl_zb_point_t* p2);

#endif