#include "sl.c.h"

static sl_context_t* context;

SL_API void sl_init()
{
    //context = 
}

SL_API void sl_close()
{
    sl_free(context);
    context = NULL;
}

sl_context_t* sl_get_context()
{
    return context;
}