#include <ks/defs.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

KS_API char* ks_strdup(const char* str)
{
    char* p;

    assert(str);

    p = malloc(strlen(str) + 1);
    strcpy(p, str);

    return p;
}
