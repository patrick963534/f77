#include <ks/string.h>
#include <ks/stdlib.h>
#include <ks/assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

KS_API void* ks_memchr(const void *str, int c, size_t n)
{
    return memchr(str, c, n);
}

KS_API int ks_memcmp(const void *str1, const void *str2, size_t n)
{
    return memcmp(str1, str2, n);
}

KS_API void* ks_memcpy(void *str1, const void *str2, size_t n)
{
    return memcpy(str1, str2, n);
}

KS_API void* ks_memmove(void *str1, const void *str2, size_t n)
{
    return memmove(str1, str2, n);
}

KS_API void* ks_memset(void *str, int c, size_t n)
{
    return memset(str, c, n);
}


KS_API char* ks_strcat(char *str1, const char *str2)
{
    return strcat(str1, str2);
}

KS_API char* ks_strncat(char *str1, const char *str2, size_t n)
{
    return strncat(str1, str2, n);
}

KS_API char* ks_strchr(const char *str, int c)
{
    return strchr(str, c);
}

KS_API int ks_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}

KS_API int ks_strncmp(const char *str1, const char *str2, size_t n)
{
    return strncmp(str1, str2, n);
}

KS_API int ks_strcoll(const char *str1, const char *str2)
{
    return strcoll(str1, str2);
}

KS_API char* ks_strcpy(char *str1, const char *str2)
{
    return strcpy(str1, str2);
}

KS_API char* ks_strncpy(char *str1, const char *str2, size_t n)
{
    return strncpy(str1, str2, n);
}

KS_API size_t ks_strcspn(const char *str1, const char *str2)
{
    return strcspn(str1, str2);
}

KS_API char* ks_strerror(int errnum)
{
    return strerror(errnum);
}

KS_API size_t ks_strlen(const char *str)
{
    return strlen(str);
}

KS_API char* ks_strpbrk(const char *str1, const char *str2)
{
    return strpbrk(str1, str2);
}

KS_API char* ks_strrchr(const char *str, int c)
{
    return strrchr(str, c);
}

KS_API size_t ks_strspn(const char *str1, const char *str2)
{
    return strspn(str1, str2);
}

KS_API char* ks_strstr(const char *str1, const char *str2)
{
    return strstr(str1, str2);
}

KS_API size_t ks_strxfrm(char *str1, const char *str2, size_t n)
{
    return strxfrm(str1, str2, n);
}

KS_API char* ks_strtok(char* str1, const char* str2)
{
    return strtok(str1, str2);
}

KS_API char* ks_strtoken(char *str1, const char* str2, char** next)
{
    ks_unused(next);
    return strtok(str1, str2);
}

KS_API char* ks_strdup(const char* str)
{
    char* p;

    ks_assert(str);

    p = malloc(ks_strlen(str) + 1);
    ks_strcpy(p, str);

    return p;
}

KS_API int ks_snprintf(char* buf, int sz, const char* format, ...)
{
    int len;

    va_list va;
    va_start(va, format);
    vsprintf(buf, format, va);
    va_end(va);

    len = strlen(buf);
    ks_assert(len < sz);

    return len;
}
