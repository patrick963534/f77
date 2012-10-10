#ifndef __KS_STDLIB_H__
#define __KS_STDLIB_H__

#include <ks/defs.h>
#include <stdlib.h>

KS_API  double              ks_atof(const char *str);
KS_API  int                 ks_atoi(const char *str);
KS_API  long int            ks_atol(const char *str);
KS_API  double              ks_strtod(const char *str, char **endptr);
KS_API  long int            ks_strtol(const char *str, char **endptr, int base);
KS_API  unsigned long int   ks_strtoul(const char *str, char **endptr, int base);

KS_API  void*               ks_calloc(size_t nitems, size_t size);
KS_API  void                ks_free(void *ptr);
KS_API  void*               ks_malloc(size_t size);
KS_API  void*               ks_realloc(void *ptr, size_t size);

KS_API  void                ks_abort();
KS_API  int                 ks_atexit(void (*func)());
KS_API  void                ks_exit(int status);
KS_API  char*               ks_getenv(const char *name);
KS_API  int                 ks_system(const char *string);

KS_API  int                 ks_abs(int x);
KS_API  div_t               ks_div(int numer, int denom);
KS_API  long int            ks_labs(long int x);
KS_API  ldiv_t              ks_ldiv(long int numer, long int denom);
KS_API  int                 ks_rand();
KS_API  void                ks_srand(unsigned int seed);

KS_API  void*   ks_bsearch(const void *key, const void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *));
KS_API  void    ks_qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));

#endif
