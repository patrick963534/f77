#include "sl.c.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

static sl_context_t* context;

SL_API void sl_init()
{
    context = calloc(1, sizeof(*context));

    context->matrix_projection = &context->matrix_stack[0];
    context->matrix_model_view = &context->matrix_stack[1];

    sl_matrix_mode(sl_matrix_mode_project);
    sl_load_identity();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();
}

SL_API void sl_close()
{
    sl_free(context);
    context = NULL;
}

void sl_error_exit(const char* format, ...)
{
    char buf[1024];
    va_list va;

    va_start(va, format);
    vsnprintf(buf, sizeof(buf), format, va);
    va_end(va);

    printf("[SL] %s\r\n", buf);
    fflush(stdout);

    exit(0);
}

sl_context_t* sl_get_context()
{
    return context;
}

