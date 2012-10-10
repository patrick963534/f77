#include <ks/defs.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

KS_API char* ks_strtoken(char *str1, const char* str2, char** next)
{
    ks_unused(next);
    return strtok(str1, str2);
}
