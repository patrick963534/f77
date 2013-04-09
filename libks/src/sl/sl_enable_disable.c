#include "sl.c.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

SL_API void sl_enable(sl_mode_e mode)
{
    sl_context_t* c = sl_context();

    switch (mode)
    {
    case sl_mode_cull_face:
        c->is_cull_face = 1;
        break;
    default:
        sl_error_exit("%s, wrong mode: %d", __FUNCTION__, mode);
        break;
    }
}

SL_API void sl_disable(sl_mode_e mode)
{
    sl_context_t* c = sl_context();

    switch (mode)
    {
    case sl_mode_cull_face:
        c->is_cull_face = 0;
        break;
    default:
        sl_error_exit("%s, wrong mode: %d", __FUNCTION__, mode);
        break;
    }
}