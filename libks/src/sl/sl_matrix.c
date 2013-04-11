#include "sl_matrix.h"
#include "sl.c.h"
#include <string.h>
#include <math.h>

static void matrix_multiply_left(sl_matrix_t* c, const sl_matrix_t* left)
{
    sl_matrix_t a = *c;
    sl_matrix_t b = *left;

    int i, j, k;
    float s;

    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j) 
        {
            s = 0;

            for(k = 0;k < 4; ++k) 
                s += b.m[i][k] * a.m[k][j];

            c->m[i][j]=s;
        }
    }
}

void sl_matrix_multiply(sl_matrix_t* c, const sl_matrix_t* left, const sl_matrix_t* right)
{
    sl_matrix_t a = *right;
    sl_matrix_t b = *left;

    int i, j, k;
    float s;

    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 4; ++j) 
        {
            s = 0;

            for(k = 0;k < 4; ++k) 
                s += b.m[i][k] * a.m[k][j];

            c->m[i][j]=s;
        }
    }
}

SL_API void sl_matrix_mode(sl_matrix_mode_e mode)
{
    sl_context_t* c = sl_context();

    switch (mode)
    {
    case sl_matrix_mode_project:
        c->matrix_mode = 0;
        break;
    case sl_matrix_mode_model:
        c->matrix_mode = 1;
        break;
    case sl_matrix_mode_texture:
        c->matrix_mode = 2;
        break;
    default:
        sl_error_exit("%s: wrong mode: %d", __FUNCTION__, (int)mode);
        break;
    }

    c->matrix_current = &c->matrix[c->matrix_mode];
}

SL_API void sl_load_identity()
{
    static sl_matrix_t identity = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };

    memcpy(sl_context()->matrix_current, &identity, sizeof(identity));
}

SL_API void sl_translate(float x, float y, float z)
{
    float* m = &sl_context()->matrix_current->m[0][0];

    m[12] = m[0] * x + m[4] * y + m[8]  * z + m[12];
    m[13] = m[1] * x + m[5] * y + m[9]  * z + m[13];
    m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
    m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];
}

SL_API void sl_rotate(float rotation, float x, float y, float z)
{
    sl_matrix_t m;
    float angle, len;
    float c, s, cx, cy, cz;

    len = x*x+y*y+z*z;
    if (len == 0.0f) 
        return;

    angle = (float)(rotation * SL_PI / 180.0);
    len = (float)(1.0f / sqrt(len));
    x *= len; y *= len; z *= len;

    c = (float)cos(angle);
    s = (float)sin(angle);

    if (c < 1E-6 && c > -1E-6)
        c = 0;
    if (s < 1E-6 && s > -1E-6)
        s = 0;

    cx = x * (1 - c); cy = y * (1 - c); cz = z * (1 - c);

    m.m[0][0] = x*cx+c;   m.m[0][1] = x*cy+z*s; m.m[0][2] = x*cz-s*y; m.m[0][3] = 0;
    m.m[1][0] = y*cx-s*z; m.m[1][1] = y*cy+c;   m.m[1][2] = y*cz+s*x; m.m[1][3] = 0;
    m.m[2][0] = z*cx+s*y; m.m[2][1] = z*cy-x*s; m.m[2][2] = z*cz+c;   m.m[2][3] = 0;
    m.m[3][0] = 0;        m.m[3][1] = 0;        m.m[3][2] = 0;        m.m[3][3] = 1;

    matrix_multiply_left(sl_context()->matrix_current, &m);
}

SL_API void sl_scale(float x, float y, float z)
{
    float* m =&sl_context()->matrix_current->m[0][0];

    m[0] *= x;  m[4] *= y;  m[8]  *= z;
    m[1] *= x;  m[5] *= y;  m[9]  *= z;
    m[2] *= x;  m[6] *= y;  m[10] *= z;
    m[3] *= x;  m[7] *= y;  m[11] *= z;
}

SL_API void sl_push_matrix()
{
    sl_context_t* c = sl_context();

    if (c->matrix_stacks_ptr[c->matrix_mode] - c->matrix_stacks[c->matrix_mode] >= Matrix_Stack_Size - 1)
    {
        sl_error_exit("matrix stack is already full.");
        return;
    }

    ++c->matrix_stacks_ptr[c->matrix_mode];
    *c->matrix_stacks_ptr[c->matrix_mode] = *c->matrix_current;
}

SL_API void sl_pop_matrix()
{
    sl_context_t* c = sl_context();

    if (c->matrix_stacks_ptr[c->matrix_mode] == c->matrix_stacks[c->matrix_mode])
    {
        sl_error_exit("matrix stack is already empty.");
        return;
    }

    *c->matrix_current = *c->matrix_stacks_ptr[c->matrix_mode];
    --c->matrix_stacks_ptr[c->matrix_mode];
}

SL_API void sl_mult_matrix(const float *m)
{
    sl_context_t* c = sl_context();
    
    matrix_multiply_left(c->matrix_current, (sl_matrix_t*)m);
}

SL_API void sl_frustum(float left, float right, float bottom, float top, float near, float far)
{
    sl_context_t* c = sl_context();
    sl_matrix_t m;

    float x = (2.0f*near) / (right-left);
    float y = (2.0f*near) / (top-bottom);
    float A = (right+left) / (right-left);
    float B = (top+bottom) / (top-bottom);
    float C = -(far+near) / ( far-near);
    float D = -(2.0f*far*near) / (far-near);

    float* r= &m.m[0][0];
    r[0]  = x; r[1]  = 0; r[2]  = 0;  r[3]  = 0;
    r[4]  = 0; r[5]  = y; r[6]  = 0;  r[7]  = 0;
    r[8]  = A; r[9]  = B; r[10] = C;  r[11] = -1;
    r[12] = 0; r[13] = 0; r[14] = D;  r[15] = 0;

    matrix_multiply_left(c->matrix_current, &m);
}

SL_API void sl_ortho(float left, float right, float bottom, float top, float near, float far)
{
    sl_context_t* c = sl_context();
    float dx = right - left;
    float sx = right + left;
    float dy = top - bottom;
    float sy = top + bottom;
    float dz = far - near;
    float sz = far + near;
    float m[16];

    m[0]  = 2/dx;   m[1]  = 0;      m[2]  = 0;      m[3]  = 0;
    m[4]  = 0;      m[5]  = 2/dy;   m[6]  = 0;      m[7]  = 0;
    m[8]  = 0;      m[9]  = 0;      m[10] = -2/dz;  m[11] = 0;
    m[12] = -sx/dx; m[13] = -sy/dy; m[14] = -sz/dz; m[15] = 1;

    matrix_multiply_left(c->matrix_current, (sl_matrix_t*)&m);
}

int sl_normalize(float* x, float* y, float* z)
{
    float xx = *x, yy = *y, zz = *z;
    float n = (float)sqrt(xx * xx + yy * yy + zz * zz);

    if (n == 0) 
        return 1;

    *x = xx / n;
    *y = yy / n;
    *z = zz / n;

    return 0;
}