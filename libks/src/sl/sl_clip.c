#include "sl.c.h"
#include <math.h>

static float clip_factor(float v0, float w0, float v1, float w1)
{
    float len = (v0 - w0) + (w1 - v1);

    if (len < 0)
        len = -len;

    if (len == 0)
        return 0;
    else
        return (v0 - w0) / len;        
}

static float clip_ymax(sl_vector_t *c, const sl_vector_t *a, const sl_vector_t *b) 
{
    float t, dX, dY, dZ, dW, den;

    dX = (b->x - a->x);
    dY = (b->y - a->y);
    dZ = (b->z - a->z);
    dW = (b->w - a->w);
    den =  - (+ dY) + dW;

    if (den == 0) 
        t = 0;
    else 
        t = (+ a->y - a->w) / den;

    c->x = a->x + t * dX;
    c->z = a->z + t * dX;
    c->w = a->w + t * dW;
    c->y = a->y + t * dY; //+ c->w;

    return t;
}

static void calc_clip_code(sl_vertex_t* v)
{
    float w = v->prj.w;
    if (w < 0)
        w = -w;

    w += 0.00001f;
    v->clip_code = (v->prj.x >  w)      |
                   (v->prj.x < -w) << 1 |
                   (v->prj.y >  w) << 2 |
                   (v->prj.y < -w) << 3 |
                   (v->prj.z >  w) << 4 |
                   (v->prj.z < -w) << 5;
}

static void find_clip_vertex(sl_vertex_t* c, const sl_vertex_t* a, const sl_vertex_t* b, int a_bit)
{
    float t = 0, len;
    float av, aw, bv, bw;

    switch (a_bit)
    {
    case 0: av = a->prj.x; aw =  a->prj.w; bv = b->prj.x; bw =  b->prj.w; break;
    case 1: av = a->prj.x; aw = -a->prj.w; bv = b->prj.x; bw = -b->prj.w; break;
    case 2: av = a->prj.y; aw =  a->prj.w; bv = b->prj.y; bw =  b->prj.w; break;
    case 3: av = a->prj.y; aw = -a->prj.w; bv = b->prj.y; bw = -b->prj.w; break;
    case 4: av = a->prj.z; aw =  a->prj.w; bv = b->prj.z; bw =  b->prj.w; break;
    case 5: av = a->prj.z; aw = -a->prj.w; bv = b->prj.z; bw = -b->prj.w; break;
    default: sl_error_exit("wrong triangle clipping."); return;
    }

    len = (av - aw) + (bw - bv);
    if (len != 0)
        t = (av - aw) / len;

    c->prj.x = a->prj.x - t * (a->prj.x - b->prj.x);
    c->prj.y = a->prj.y - t * (a->prj.y - b->prj.y);
    c->prj.z = a->prj.z - t * (a->prj.z - b->prj.z);
    c->prj.w = a->prj.w - t * (a->prj.w - b->prj.w);
}

static void draw_triangle_clip(sl_vertex_t* v0, sl_vertex_t* v1, sl_vertex_t* v2, int bit) 
{
    int c0 = v0->clip_code;
    int c1 = v1->clip_code;
    int c2 = v2->clip_code;
    int b1 = c0 | c1 | c2;

    if (b1 == 0)
    {
        sl_rasterize(&v0->zp, &v1->zp, &v2->zp);
    }
    else
    {
        sl_vertex_t *p0, *p1, *p2;

        sl_vertex_t  tv1, tv2;
        int out_cn, mask;

        int b2 = c0 & c1 & c2;

        if (b2 == 0)
            return;

        while ((b2 & (1 << bit)) == 0 && bit < 6)
            ++bit;

        if (bit >= 6)
            return;

        mask = 1 << bit;
        out_cn = (c0 ^ c1 ^ c2) & mask;

        if (out_cn) // one point out
        {
            if      (c0 & mask) { p0 = v0; p1 = v1; p2 = v2; }
            else if (c1 & mask) { p0 = v1; p1 = v2; p2 = v0; }
            else                { p0 = v2; p1 = v0; p2 = v1; }

            find_clip_vertex(&tv1, p0, p1, bit);
            draw_triangle_clip(&tv1, p1, p2, bit+1);   

            find_clip_vertex(&tv2, p0, p2, bit);
            draw_triangle_clip(&tv2, &tv1, p2, bit+1);   
        }
        else // two points out
        {
            if      ((c0 & mask) == 0) { p0 = v0; p1 = v1; p2 = v2; }
            else if ((c1 & mask) == 0) { p0 = v1; p1 = v2; p2 = v0; }
            else                       { p0 = v2; p1 = v0; p2 = v1; }

            find_clip_vertex(&tv1, p0, p1, bit);
            find_clip_vertex(&tv2, p0, p2, bit);

            draw_triangle_clip(p0, &tv1, &tv2, bit+1);   
        }
    }
}

void sl_clip_draw()
{
    sl_context_t* c = sl_context();
    sl_vertex_t* v0 = &c->vertexs[0];
    sl_vertex_t* v1 = &c->vertexs[1];
    sl_vertex_t* v2 = &c->vertexs[2];

    calc_clip_code(v0);
    calc_clip_code(v1);
    calc_clip_code(v2);

    draw_triangle_clip(v0, v1, v2, 0);
}