#ifndef __KS_DEFS_H__
#define __KS_DEFS_H__

#include <assert.h>

#define ks_assert(v, msg)   do { if(!(v)) assert(!(msg)); } while(0)

#define KS_API          extern
#define KS_INLINE       inline

#define ks_max(a, b)    ((a) > (b) ? (a) : (b))
#define ks_min(a, b)    ((a) < (b) ? (a) : (b))

#define ks_cast(T)      T* me = (T*)me_

#define ks_unused(p)    (void)(p);

#ifndef NULL
    #define NULL        ((void*)0)
#endif

#endif
