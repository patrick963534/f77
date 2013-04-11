#include "sl.c.h"
#include <sl/sl.h>
#include <stdlib.h>

SL_API void sl_tex_coord(float u, float v)
{
    sl_context_t* c = sl_context();
    c->u = u;
    c->v = v;
}

SL_API void sl_vertex(float x, float y, float z)
{
    sl_context_t* c = sl_context();
    sl_vertex_t* v = &c->vertexs[c->nvertex++];

    v->x = x; v->y = y; v->z = z;

    
}
