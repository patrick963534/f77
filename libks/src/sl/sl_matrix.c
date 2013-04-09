#include "sl_matrix.h"
#include "sl.c.h"
#include <string.h>
#include <math.h>

static int normalize(float* x, float* y, float* z)
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

    m.m[0][0] = x*cx+c;   m.m[0][1] = y*cx-s*z; m.m[0][2] = z*cx+s*y; m.m[0][3] = 0;
    m.m[1][0] = x*cy+z*s; m.m[1][1] = y*cy+c;   m.m[1][2] = z*cy-x*s; m.m[1][3] = 0;
    m.m[2][0] = x*cz-s*y; m.m[2][1] = y*cz+s*x; m.m[2][2] = z*cz+c;   m.m[2][3] = 0;
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
