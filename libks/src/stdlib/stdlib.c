#include <ks/stdlib.h>

KS_API double ks_atof(const char *str)
{
    return atof(str);
}

KS_API int ks_atoi(const char *str)
{
    return atoi(str);
}

KS_API long int ks_atol(const char *str)
{
    return atol(str);
}

KS_API double ks_strtod(const char *str, char **endptr)
{
    return strtod(str, endptr);
}

KS_API long int ks_strtol(const char *str, char **endptr, int base)
{
    return strtol(str, endptr, base);
}

KS_API unsigned long int ks_strtoul(const char *str, char **endptr, int base)
{
    return strtoul(str, endptr, base);
}

KS_API void ks_abort()
{
    abort();
}

KS_API int ks_atexit(void (*func)())
{
    return atexit(func);
}

KS_API void ks_exit(int status)
{
    exit(status);
}

KS_API char* ks_getenv(const char *name)
{
    return getenv(name);
}

KS_API int ks_system(const char *string)
{
    return system(string);
}

KS_API int ks_abs(int x)
{
    return abs(x);
}

KS_API div_t ks_div(int numer, int denom)
{
    return div(numer, denom);
}

KS_API long int ks_labs(long int x)
{
    return labs(x);
}

KS_API ldiv_t ks_ldiv(long int numer, long int denom)
{
    return ldiv(numer, denom);
}

KS_API int ks_rand()
{
    return rand();
}

KS_API void ks_srand(unsigned int seed)
{
    srand(seed);
}

KS_API void* ks_bsearch(const void *key, const void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *))
{
    return bsearch(key, base, nitems, size, compar);
}

KS_API void ks_qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
{
    return qsort(base, nitems, size, compar);
}

