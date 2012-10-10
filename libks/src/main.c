#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>

int main()
{
    ks_time_t t1, t2;

    ks_log("%d\n", sizeof(long int));

    t1 = ks_time_now();

    while (1)
    {
        ks_time_sleep(1000);

        t2 = ks_time_now();
        ks_log("%d ", ks_time_differ_in_msec(t2, t1));
        t1 = t2;
    }

    return 0;
}
