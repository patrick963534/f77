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

typedef struct sl_context_t
{
    sl_matrix_t     matrix[3];
    sl_matrix_t*    matrix_projection;
    sl_matrix_t*    matrix_model_view;
    sl_matrix_t*    matrix_texture;
    sl_matrix_t*    matrix_current;
    int             matrix_mode;
    
    sl_matrix_t     matrix_stacks[3][Matrix_Stack_Size];
    sl_matrix_t*    matrix_stacks_ptr[3];

    int             is_cull_face;
} sl_context_t;

void*         sl_malloc(int sz);
void*         sl_calloc(int count, int sz);
void          sl_free(void* p);

void          sl_error_exit(const char* format, ...);
sl_context_t* sl_context();
int           sl_normalize(float* x, float* y, float* z);

#endif