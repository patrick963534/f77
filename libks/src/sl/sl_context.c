#include "sl.c.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

static sl_context_t* context;

SL_API void sl_init()
{
    int i;

    context = calloc(1, sizeof(*context));

    context->matrix_projection = context->matrix + 0;
    context->matrix_model_view = context->matrix + 1;
    context->matrix_texture    = context->matrix + 2;

    sl_matrix_mode(sl_matrix_mode_project);
    sl_load_identity();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();

    for (i = 0; i < 3; ++i)
        context->matrix_stacks_ptr[i] = context->matrix_stacks[i];


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

sl_context_t* sl_context()
{
    return context;
}