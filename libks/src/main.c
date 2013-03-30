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
#include <ks/event.h>
#include <ks/text.h>
#include <ks/libc.h>
#include <ks/constants.h>
#include "utest/utest.h"
#include "r7/r7_zbuffer.h"
#include <r7/gl.h>


static void step(ks_node_t* me, int delta)
{
    static float offset = 0;

    offset += delta * 5 / 1000.0f;

    if (offset > 1)
    {
        me->y += (int)offset;
        offset -= (int)offset;
    }
}

static void step_rotate(ks_node_t* me, int delta)
{
    static float angle = 0;

    angle += delta * 45 / 1000.0f;

    if (angle > 1)
    {
        me->angle += (int)angle;
        angle -= (int)angle;
    }
}


static void step_fps(ks_text_t* me, int delta)
{
    static float ellapse = 0;

    ellapse += delta;

    if (ellapse > 1000)
    {
        char buf[128];

        ks_text_set_content(me, ks_itoa(buf, sizeof(buf), ks_director_instance()->fps));
        me->x = me->image->width / 2;
        me->y = ks_director_instance()->height - me->image->height / 2;

        ellapse = 0;
    }
}

static int msgs(ks_node_t* me, ks_event_t* e)
{
    if (e->type == ks.types.KEY_DOWN)
    {
        if (e->key.code == ks.keys.Up)
            me->y += 20;
        else if (e->key.code == ks.keys.Down)
            me->y -= 20;
        else if (e->key.code == ks.keys.Left)
            me->x -= 20;
        else if (e->key.code == ks.keys.Right)
            me->x += 20;
    }

    return 0;
}

static ks_scene_t* create_scene()
{
    ks_scene_t* me;
    ks_node_t*  node;
    ks_actor_t* actor0;
    ks_actor_t* actor1;
    ks_actor_t* actor2;

    me       = ks_scene_new(sizeof(*me));
    me->msgs = msgs;

//     node = ks_node_new(sizeof(*node), (ks_node_t*)me);
//     node->x = 100;
// 
//     ks_text_new("arial.ttf", 18, 60, "",        node)->step = (ks_step_f)step_fps;
//     ks_text_new("arial.ttf", 18, 60, "0",       node)->y = 200;
//     ks_text_new("arial.ttf", 18, 60, "00",      node)->y = 60;
//     ks_text_new("arial.ttf", 18, 60, "000",     node)->y = 90;
//     ks_text_new("arial.ttf", 18, 60, "0000",    node)->y = 120;
//     ks_text_new("arial.ttf", 18, 60, "00000",   node)->y = 150;

    actor0 = ks_actor_new("img.png", (ks_node_t*)me);
    actor0->x = 400;
    actor0->y = 240;
    actor0->angle = 45;
    actor0->step = step_rotate;

//     actor1 = ks_actor_new("img.png", (ks_node_t*)me);
//     actor1->x = 384 + 10;
//     actor1->y = 256;

//     actor2 = ks_actor_new("img.png", (ks_node_t*)me);
//     actor2->x = 640 + 20;
//     actor2->y = 256;

    //actor0->step = step;

    return me;
}

int main(int argc, char** argv)
{
    ks_director_init("Hello World", 800, 480, argc, argv);

    ks_utest_start();

    ks_director_run(create_scene());
    return 0;
}
