#include "sl.c.h"
#include "sl_matrix.h"
#include <sl/sl.h>
#include <stdlib.h>

static void apply_mvp(sl_vertex_t* v)
{
    float* m = &sl_context()->matrix_model_view_projection.m[0][0];

    v->prj.x = (v->pos.x * m[0] + v->pos.y * m[4] + v->pos.z * m[8]  + m[12]);
    v->prj.y = (v->pos.x * m[1] + v->pos.y * m[5] + v->pos.z * m[9]  + m[13]);
    v->prj.z = (v->pos.x * m[2] + v->pos.y * m[6] + v->pos.z * m[10] + m[14]);
    v->prj.w = (v->pos.x * m[3] + v->pos.y * m[7] + v->pos.z * m[11] + m[15]);
}

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

    apply_mvp(v);

    if (c->nvertex == 3)
        sl_clip_draw();
}
