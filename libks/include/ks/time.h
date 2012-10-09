#ifndef __KS_TIME_H__
#define __KS_TIME_H__

#include <ks/defs.h>

/* get milli-seconds from year 1970 to now. */
KS_API int      ks_time_get();
KS_API void     ks_time_sleep(int ms);

#endif
