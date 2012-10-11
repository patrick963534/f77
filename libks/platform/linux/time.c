#include <ks/defs.h>
#include <ks/time.h>
#include <unistd.h>
#include <time.h>

struct ks_time_t
{
};

KS_API ks_time_t ks_time_now()
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);

    return t.tv_sec * 1000000 + t.tv_nsec / 1000;
}

KS_API int ks_time_differ_in_msec(ks_time_t new, ks_time_t old)
{
    return (int)((new - old) / 1000);
}

KS_API void ks_time_sleep(int msec)
{
    if (msec < 0)
        return;

    usleep(msec * 1000);
}

