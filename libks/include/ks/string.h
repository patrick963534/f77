#ifndef __KS_STRING_H__
#define __KS_STRING_H__

#include <ks/defs.h>
#include <stdio.h>

KS_API void*    ks_memchr(const void *str, int c, size_t n);
KS_API int      ks_memcmp(const void *str1, const void *str2, size_t n);
KS_API void*    ks_memcpy(void *str1, const void *str2, size_t n);
KS_API void*    ks_memmove(void *str1, const void *str2, size_t n);
KS_API void*    ks_memset(void *str, int c, size_t n);

KS_API char*    ks_strcat(char *str1, const char *str2);
KS_API char*    ks_strncat(char *str1, const char *str2, size_t n);
KS_API char*    ks_strchr(const char *str, int c);
KS_API int      ks_strcmp(const char *str1, const char *str2);
KS_API int      ks_strncmp(const char *str1, const char *str2, size_t n);
KS_API int      ks_strcoll(const char *str1, const char *str2);
KS_API char*    ks_strcpy(char *str1, const char *str2);
KS_API char*    ks_strncpy(char *str1, const char *str2, size_t n);
KS_API size_t   ks_strcspn(const char *str1, const char *str2);
KS_API char*    ks_strerror(int errnum);
KS_API size_t   ks_strlen(const char *str);
KS_API char*    ks_strpbrk(const char *str1, const char *str2);
KS_API char*    ks_strrchr(const char *str, int c);
KS_API size_t   ks_strspn(const char *str1, const char *str2);
KS_API char*    ks_strstr(const char *str1, const char *str2);
KS_API size_t   ks_strxfrm(char *str1, const char *str2, size_t n);

/**
 disable 'strtok', please use custom 'strtoken' instead.
KS_API char*    ks_strtok(char *str1, const char *str2);
*/

KS_API char*    ks_strtoken(char *str1, const char* str2, char** next);
KS_API char*    ks_strdup(const char* str);
KS_API int      ks_snprintf(char* buf, int sz, const char* format, ...);

#endif
