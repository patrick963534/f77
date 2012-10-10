#include <ks/defs.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

KS_API int ks_snprintf(char* buf, int sz, const char* format, ...)
{
    int len;

    va_list va;
    va_start(va, format);
    vsprintf(buf, format, va);
    va_end(va);

    len = strlen(buf);
    assert(len < sz);

    return len;
}
