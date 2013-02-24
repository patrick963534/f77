#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>
#include <ks/system.h>
#include <stdlib.h>
#include <string.h>
#include <ks/director.h>
#include <ks/scene.h>
#include <ks/actor.h>
#include <ks/helper.h>
#include "utest/utest.h"

static ks_scene_t* create_scene()
{
    char buf[256];

    ks_scene_t* me;
    ks_actor_t* actor0;
    ks_actor_t* actor1;
    ks_actor_t* actor2;

    ks_helper_path_join_relative_app(buf, sizeof(buf), "img.png");

    me = ks_scene_new(sizeof(*me));
    actor0 = ks_actor_new(buf, sizeof(*actor0), (ks_node_t*)me);
    actor1 = ks_actor_new(buf, sizeof(*actor1), (ks_node_t*)me);
    actor2 = ks_actor_new(buf, sizeof(*actor2), (ks_node_t*)me);

    actor0->x = 0;
    actor1->x = 400;
    actor2->x = 800;

    return me;
}

int main(int argc, char** argv)
{
    ks_director_init("Hello World", 320, 240, argc, argv);

    ks_utest_start();

    ks_director_run(create_scene());
    return 0;
}
