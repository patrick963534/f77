#ifndef __SKY_LIBRARY_MATRIX_H__
#define __SKY_LIBRARY_MATRIX_H__

typedef struct sl_matrix_t
{
    float m[4][4];    
} sl_matrix_t;

typedef struct sl_vector_t
{
    float x, y, z, w;
} sl_vector_t;

void sl_matrix_multiply(sl_matrix_t* c, const sl_matrix_t* left, const sl_matrix_t* right);

#endif