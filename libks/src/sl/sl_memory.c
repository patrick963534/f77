#include "sl.c.h"
#include <stdlib.h>

void* sl_malloc(int sz)
{
    return malloc(sz);
}

void* sl_calloc(int count, int sz)
{
    return calloc(count, sz);
}

void sl_free(void* p)
{
    free(p);
}