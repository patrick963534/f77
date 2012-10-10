#include <ks/log.h>
#include <stdio.h>
#include <stdarg.h>

KS_API void ks_print_log(const char *type, const char *format, ...)
{
    char buf[256];
    va_list va;

    va_start(va, format);
    vsnprintf(buf, sizeof(buf), format, va);
    va_end(va);

    printf("[%s] %s\r\n", type, buf);
    fflush(stdout);
}
