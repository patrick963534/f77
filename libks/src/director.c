#include <ks/director.h>
#include <ks/system.h>
#include <ks/time.h>
#include <ks/event.h>
#include <ks/eventq.h>
#include <ks/graphics.h>
#include <ks/log.h>
#include <ks/constants.h>
#include <ks/helper.h>

#include <string.h>

#include "environment.c.h"

static ks_director_t* director;
static ks_image_t* img;

static void director_update()
{

}

static void director_draw()
{
    if (!img)
    {
        char buf[256];

        ks_helper_path_join_relative_app(buf, sizeof(buf), "img.png");
        img = ks_image_load(buf, NULL);
        ks_log("image size: %d, %d", img->width, img->height);
    }

    ks_graphics_load_identity();
    ks_graphics_clear_screen();
    ks_graphics_translate(0, 150);
    ks_graphics_draw(img, 0, 0, img->width, img->height);
    ks_system_flush();
}

static void destruct(ks_director_t* me)
{
    ks_object_destruct((ks_object_t*)me);
}

KS_API void ks_director_init(const char* title, int w, int h, int argc, char** argv)
{
    director = (ks_director_t*)ks_object_new(sizeof(*director));
    director->destruct = (ks_destruct_f)destruct;
    director->width = w;
    director->height = h;
    strcpy(director->title, title);

    ks_environment_init(argc, argv, (ks_object_t*)director);
}

KS_API void ks_director_run(ks_scene_t* scene)
{
    ks_time_t t1, t2;
    ks_event_t e;
    int ellapse = 0;
    int count = 0;

    ks_system_init((ks_object_t*)director);

    ks_log("%d\n", sizeof(long int));

    t1 = ks_time_now();

    while (count < 10000)
    {
        ks_system_update_message();

        if (ks_eventq_pop(&e))
        {
            if (e.type == ks.types.KEY_DOWN && e.key.code == ks.keys.Escape)
                break;

            continue;
        }

        count++;

        ks_time_sleep(1);

        director_update();
        director_draw();

        t2 = ks_time_now();
        ellapse += ks_time_differ_in_msec(t2, t1);

        if (ellapse > 1000)
        {
            ks_log("FPS: %d ", count);
            ellapse = 0;
            count = 0;
        }

        t1 = t2;
    }

    ks_unused(scene);

    ks_object_delete(director);
}

KS_API ks_director_t* ks_director_instance()
{
    return director;
}
