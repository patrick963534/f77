#ifndef __KS_DEFS_H__
#define __KS_DEFS_H__

#define KS_API          extern

#define ks_max(a, b)    ((a) > (b) ? (a) : (b))
#define ks_min(a, b)    ((a) < (b) ? (a) : (b))

#define ks_cast(T)      T* me = (T*)me_

#define ks_unused(p)    (void)(p);

#endif
