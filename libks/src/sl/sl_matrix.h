#ifndef __SKY_LIBRARY_MATRIX_H__
#define __SKY_LIBRARY_MATRIX_H__

typedef struct sl_matrix_t
{
    float m[4][4];    
} sl_matrix_t;

typedef struct sl_vector_t
{
    float x, y, z;
} sl_vector_t;

#endif