#ifndef __SKY_LIBRARY_C_H__
#define __SKY_LIBRARY_C_H__

#include <sl/sl.h>
#include "sl_matrix.h"

#define NULL                ((void*)0)
#define sl_unused(a)        (void)(a)

typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned char   uchar;

typedef struct sl_context_t
{
    sl_matrix_t     matrix_projection;
    sl_matrix_t     matrix_model_view;
    sl_matrix_t*    matrix_current;
    

} sl_context_t;

void*         sl_malloc(int sz);
void*         sl_calloc(int count, int sz);
void          sl_free(void* p);

void          sl_error_exit(const char* format, ...);
sl_context_t* sl_get_context();


#endif