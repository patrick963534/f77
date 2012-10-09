#ifndef __KS_TIME_H__
#define __KS_TIME_H__

#include <ks/defs.h>

typedef struct ks_time_t ks_time_t;

KS_API ks_time_t    ks_time_now();
KS_API int          ks_time_differ_in_msec(ks_time_t new, ks_time_t old);
KS_API void         ks_time_sleep(int msec);

#endif
