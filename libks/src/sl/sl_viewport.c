#include "sl.c.h"
#include <sl/sl.h>
#include <stdlib.h>

void sl_transform_viewport(sl_vertex_t* v)
{
    sl_context_t* c = sl_context();
    double w_inv;

    w_inv = (1.0 / v->prj.w);
    v->zp.x = (int)(v->prj.x * w_inv * c->viewport.scale.x + c->viewport.trans.x);
    v->zp.y = (int)(v->prj.y * w_inv * c->viewport.scale.y + c->viewport.trans.y);
    v->zp.z = (int)(v->prj.z * w_inv * c->viewport.scale.z + c->viewport.trans.z);
 
    v->zp.u = v->u;
    v->zp.v = v->v;
}

SL_API void sl_viewport(int x, int y, int w, int h)
{
    sl_context_t* c = sl_context();
    sl_viewport_t* v = &c->viewport;
    float zsize = (1 << (16 + 14));

    if (w <= 0 || h <= 0 || (x + w) <= 0 || (y + h) <= 0)
        sl_error_exit("wrong parameter values.");

    if (v->x != x || v->y != y || v->w != w || v->h != h) 
    {
        v->x = x; v->y = y; v->w = w; v->h = h;

        v->trans.x = ((v->w - 0.5f) / 2.0f) + v->x;
        v->trans.y = ((v->h - 0.5f) / 2.0f) + v->y;
        v->trans.z = ((zsize - 0.5f) / 2.0f) + ((1 << 14)) / 2;

        v->scale.x =  (v->w - 0.5f) / 2.0f;
        v->scale.y = -(v->h - 0.5f) / 2.0f;
        v->scale.z = -((zsize - 0.5f) / 2.0f);

        {
            int zw = max(x + w, w);
            int zh = max(y + h, h);

            free(c->zbuffer.pbuf);
            free(c->zbuffer.zbuf);
            c->zbuffer.pbuf = sl_calloc(1, zw * zh * sizeof(c->zbuffer.pbuf[0]));
            c->zbuffer.zbuf = sl_calloc(1, zw * zh * sizeof(c->zbuffer.zbuf[0]));
            c->zbuffer.w = zw;
            c->zbuffer.w = zh;
        }        
    }
}