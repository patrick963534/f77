#include <ks/director.h>
#include <ks/sys/system.h>
#include "ks/time.h"
#include "ks/sys/graphics.h"
#include "ks/log.h"

static ks_director_t* director;

static void director_update()
{

}

static void director_draw()
{
    ks_sys_graphics_instance()->klass->draw(0, 0);
    ks_sys_system_instance()->klass->flush();
}

KS_API void ks_director_init(const char* title, int w, int h)
{
    director = (ks_director_t*)ks_container_new(sizeof(*director));
    director->width = w;
    director->height = h;

    ks_sys_system_init((ks_container_t*)director);
    ks_sys_system_instance()->klass->create(title, w, h);

    ks_sys_graphics_init((ks_container_t*)director);
}

KS_API void ks_director_run(ks_scene_t* scene)
{
    ks_time_t t1, t2;
    int ellapse = 0;
    int count = 0;

    ks_log("%d\n", sizeof(long int));

    t1 = ks_time_now();

    while (count < 100000)
    {
        count++;

        ks_time_sleep(32);

        director_update();
        director_draw();

        t2 = ks_time_now();
        ellapse += ks_time_differ_in_msec(t2, t1);

        if (ellapse > 1000)
        {
            ks_log("%d ", count);
            ellapse = 0;
            count = 0;
        }

        t1 = t2;
    }

    ks_unused(scene);



}

KS_API ks_director_t* ks_director_instance()
{
    return director;
}