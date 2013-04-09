#include "sl.c.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

SL_API void sl_get_floatv(sl_param_name_e pname, float* v)
{
    sl_context_t* c = sl_context();

    switch (pname)
    {
    case sl_param_name_matrix_projection:
        memcpy(v, c->matrix_projection, sizeof(sl_matrix_t));
        break;
    case sl_param_name_matrix_model_view:
        memcpy(v, c->matrix_model_view, sizeof(sl_matrix_t));
        break;
    default:
        sl_error_exit("%s, wrong pname: %d", __FUNCTION__, (int)pname);
        break;
    }
}