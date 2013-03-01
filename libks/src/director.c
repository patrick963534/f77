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
#include "scene.c.h"

static ks_director_t* director;

static void calculate_fps(int delta)
{
    static int count = 0;
    static int ellapse = 0;

    count++;
    ellapse += delta;
    if (ellapse > 1000)
    {
        ks_log("FPS: %d ", count);
        ellapse = 0;
        count = 0;
    }
}

static void director_event(ks_event_t* e)
{
    if (director->scene->msgs && director->scene->msgs((ks_node_t*)director->scene, e))
        return;

    if (e->type == ks.types.KEY_DOWN && e->key.code == ks.keys.Escape)
        director->end = 1;
}


static void director_update(int delta)
{
    so_scene_step(director->scene, delta);
}

static int calc_delta()
{
    static ks_time_t t1 = 0;
    ks_time_t t2 = ks_time_now();
    int delta;

    if (t1 == 0)
        t1 = ks_time_now();

    delta = ks_time_differ_in_msec(t2, t1);
    t1 = t2;

    return delta;
}

static void director_draw()
{
    ks_graphics_load_identity();
    ks_graphics_clear_screen();

    so_scene_draw(director->scene);

    ks_system_flush();
}

static void destruct(ks_director_t* me)
{
    ks_object_delete(director->scene);
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
    ks_event_t e;
    int delta;

    ks_system_init((ks_object_t*)director);

    director->scene = scene;

    while (!director->end)
    {
        ks_system_update_message();

        if (ks_eventq_pop(&e))
        {
            director_event(&e);
            continue;
        }

        delta = calc_delta();
        director_update(delta);
        director_draw();

        calculate_fps(delta);
        //ks_time_sleep(1);
    }

    ks_object_delete(director);
}

KS_API ks_director_t* ks_director_instance()
{
    return director;
}
