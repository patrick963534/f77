#include "sl.c.h"
#include <sl/sl.h>
#include <string.h>

SL_API void slut_get_mvp(float* m)
{
    memcpy(m, &sl_context()->matrix_model_view_projection, sizeof(sl_matrix_t));
}