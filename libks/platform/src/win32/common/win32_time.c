#include <ks/time.h>
#include <ks/defs.h>
#include <windows.h>

KS_API ks_time_t ks_time_now()
{
    LARGE_INTEGER t;   
    LARGE_INTEGER f;

    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);

	return (ks_time_t)(1000 * t.QuadPart / f.QuadPart);
}

KS_API int ks_time_differ_in_msec(ks_time_t cur, ks_time_t old)
{
	return (int)(cur - old);
}

KS_API void ks_time_sleep(int msec)
{
    if (msec < 0)
        return;

    Sleep(msec);
}

