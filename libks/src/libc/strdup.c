#include <ks/defs.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

KS_API char* ks_itoa(char* buf, int sz, int v)
{
    int cn = 1;
    int t = v;
    int i;

    while (t >= 10)
    {
        t /= 10;
        cn++;
    }

    if (cn >= sz - 1)
    {
        buf[0] = '\0';
        return buf;
    }
    
    t = v;
    for (i = 0; i < cn; ++i)
    {
        buf[cn - 1 - i] = 0x30 + t % 10;
        t /= 10;
    }

    buf[cn] = '\0';

    return buf;
}

KS_API char* ks_strdup(const char* str)
{
    char* p;

    assert(str);

    p = malloc(strlen(str) + 1);
    strcpy(p, str);

    return p;
}
