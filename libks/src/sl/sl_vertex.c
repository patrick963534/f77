#include "sl.c.h"
#include "sl_matrix.h"
#include <sl/sl.h>
#include <stdlib.h>

SL_API void sl_tex_coord(float u, float v)
{
    sl_context_t* c = sl_context();
    c->u = u;
    c->v = v;
}

SL_API void sl_begin()
{
    sl_context_t* c = sl_context();
    c->begin = 1;

    sl_matrix_multiply(&c->matrix_model_view_projection, c->matrix_model_view, c->matrix_projection);
}

SL_API void sl_end()
{
    sl_context_t* c = sl_context();
    c->begin = 0;
}

SL_API void sl_vertex(float x, float y, float z)
{
    sl_context_t* c = sl_context();
    sl_vertex_t* v;

    if (!c->begin)
        sl_error_exit("[SL_ERROR] please call sl_begin() before sl_vertex(...).");
    
    v = &c->vertexs[c->nvertex++];
    v->x = x; v->y = y; v->z = z;

    
}
