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

    v->pos.x = x; v->pos.y = y; v->pos.z = z; v->pos.w = 1.0f;

    v->u = c->u; v->v = c->v;

    sl_vector_mul_matrix(&v->prj, &v->pos, &c->matrix_model_view_projection);
    sl_transform_viewport(v);

    if (c->nvertex == 3)
    {
        sl_clip_draw();
        c->nvertex = 0;
    }
}
