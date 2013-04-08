#include "sl_matrix.h"
#include "sl.c.h"
#include <string.h>

SL_API void sl_matrix_mode(sl_matrix_mode_e mode)
{
    switch (mode)
    {
    case sl_matrix_mode_project:
        sl_get_context()->matrix_current = sl_get_context()->matrix_projection;
        break;
    case sl_matrix_mode_model:
        sl_get_context()->matrix_current = sl_get_context()->matrix_model_view;
        break;
    default:
        sl_error_exit("%s: wrong mode: %d", __FUNCTION__, (int)mode);
        break;
    }
}

SL_API void sl_load_identity()
{
    static sl_matrix_t identity = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };

    memcpy(sl_get_context()->matrix_current, &identity, sizeof(identity));
}

