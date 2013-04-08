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
    switch (mode)
    {
    case sl_matrix_mode_project:
        sl_get_context()->matrix_current = &sl_get_context()->matrix_projection;
        break;
    case sl_matrix_mode_model:
        sl_get_context()->matrix_current = &sl_get_context()->matrix_model_view;
        break;
    default:
        sl_error_exit("%s: wrong mode: %d", __FUNCTION__, (int)mode);
        break;
    }
}

SL_API void sl_load_identity()
{
    static sl_matrix_t identity = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };

    memcpy(sl_get_context()->matrix_current, &identity, sizeof(identity));
}

SL_API void sl_translate(float x, float y, float z)
{
    float* m = &sl_get_context()->matrix_current->m;

    m[12] = m[0] * x + m[4] * y + m[8]  * z + m[12];
    m[13] = m[1] * x + m[5] * y + m[9]  * z + m[13];
    m[14] = m[2] * x + m[6] * y + m[10] * z + m[14];
    m[15] = m[3] * x + m[7] * y + m[11] * z + m[15];
}

SL_API void sl_rotate(float rotation, float x, float y, float z)
{
    sl_matrix_t m;
    float u[3];
    float angle;
    float cost, sint;
    float len;

    len = x*x+y*y+z*z;
    if (len == 0.0f) 
        return;

    angle = (float)(rotation * M_PI / 180.0);
    len = (float)(1.0f / sqrt(len));
    x *= len;
    y *= len;
    z *= len;

    cost = (float)cos(angle);
    sint = (float)sin(angle);

    ///* fill in the values */
    //m.m[3][0]=m.m[3][1]=m.m[3][2]=m.m[0][3]=m.m[1][3]=m.m[2][3]=0.0f;
    //m.m[3][3]=1.0f;

    ///* do the math */
    //m.m[0][0]=x*x+cost*(1-x*x);
    //m.m[1][0]=x*y*(1-cost)-z*sint;
    //m.m[2][0]=z*x*(1-cost)+y*sint;
    //m.m[0][1]=x*y*(1-cost)+z*sint;
    //m.m[1][1]=y*y+cost*(1-y*y);
    //m.m[2][1]=y*z*(1-cost)-x*sint;
    //m.m[0][2]=z*x*(1-cost)-y*sint;
    //m.m[1][2]=y*z*(1-cost)+x*sint;
    //m.m[2][2]=z*z+cost*(1-z*z);
}

SL_API void sl_scale(float x, float y, float z)
{

}

SL_API void sl_push_matrix()
{

}

SL_API void sl_pop_matrix()
{

}
