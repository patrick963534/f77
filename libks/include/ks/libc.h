#ifndef __KS_LIBC_H__
#define __KS_LIBC_H__

#include <ks/defs.h>

KS_API int      ks_snprintf(char* buf, int sz, const char* format, ...);
KS_API char*    ks_strdup(const char* str);
KS_API char*    ks_strtoken(char *str1, const char* str2, char** next);

#endif
