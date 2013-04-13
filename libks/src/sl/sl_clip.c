#include "sl.c.h"
#include <math.h>

// static float clip_ymin(V4 *c, const V4 *a, const V4 *b) 
// {
//     float t, dX, dY, dZ, dW, den;
//     dX = (b->X - a->X);
//     dY = (b->Y - a->Y);
//     dZ = (b->Z - a->Z);
//     dW = (b->W - a->W);
//     den =  - (+ dY) + dW;
//     if (den == 0) t = 0;
//     else t = (+ a->Y - a->W) / den;
//     c->X = a->X + t * d ## dir1;
//     c->Z = a->Z + t * d ## dir2;
//     c->W = a->W + t * dW;
//     c->Y = + c->W;
//     return t;
// }

static int get_clip_code(sl_vertex_t* v)
{
    static int epsilon = 0.00001f;
    float w = abs(v->prj.w) + epsilon;

    return (v->prj.x >  w)      |
           (v->prj.x < -w) << 1 |
           (v->prj.y >  w) << 2 |
           (v->prj.y < -w) << 3 |
           (v->prj.z >  w) << 4 |
           (v->prj.z < -w) << 5;
}

void sl_clip_draw()
{
    sl_context_t* c = sl_context();
    sl_vertex_t* v0 = &c->vertexs[0];
    sl_vertex_t* v1 = &c->vertexs[1];
    sl_vertex_t* v2 = &c->vertexs[2];

    int c0 = get_clip_code(v0);
    int c1 = get_clip_code(v1);
    int c2 = get_clip_code(v2);

    int bc = c0 | c1 | c2;
    
    if (bc == 0)
        sl_rasterize(&v0->zp, &v1->zp, &v2->zp);
    else
        bc = bc;
}