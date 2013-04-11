#include "sl.c.h"



void sl_clip_draw()
{
    sl_context_t* c = sl_context();
    sl_vertex_t* v0 = &c->vertexs[0];
    sl_vertex_t* v1 = &c->vertexs[1];
    sl_vertex_t* v2 = &c->vertexs[2];

    sl_rasterize(&v0->zp, &v1->zp, &v2->zp);
}