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

#include <vld.h>

#define Win_W   1024
#define Win_H   768

static ks_actor_t* actor0;
static ks_actor_t* actor1;
static ks_actor_t* actor2;

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
    static float time = 0;

    time += delta;

    if (time > 3000)
    {
        me->angle += 20;
        time -= 3000;
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
            actor0->angle += 20;
        else if (e->key.code == ks.keys.Down)
            actor0->angle -= 20;
        else if (e->key.code == ks.keys.Left)
        {actor0->sx -= 0.1f; actor0->sy -= 0.1f; }
        else if (e->key.code == ks.keys.Right)
        {actor0->sx += 0.1f; actor0->sy += 0.1f; }
    }

    return 0;
}

static ks_scene_t* create_scene()
{
    ks_scene_t* me;
    ks_node_t*  node;

    me       = ks_scene_new(sizeof(*me));
    me->msgs = msgs;

    node = ks_node_new(sizeof(*node), (ks_node_t*)me);
    node->x = 100;
    node->z = -1;

    ks_text_new("arial.ttf", 18, 60, "",        node)->step = (ks_step_f)step_fps;
    ks_text_new("arial.ttf", 18, 60, "0",       node)->y = 200;
    ks_text_new("arial.ttf", 18, 60, "00",      node)->y = 60;
    ks_text_new("arial.ttf", 18, 60, "000",     node)->y = 90;
    ks_text_new("arial.ttf", 18, 60, "0000",    node)->y = 120;
    ks_text_new("arial.ttf", 18, 60, "00000",   node)->y = 150;

    actor0 = ks_actor_new("bag.png", (ks_node_t*)me);
    actor0->x = Win_W/2;
    actor0->y = Win_H/2;

//      actor0->sx = 0.333f;
//      actor0->sy = 1.5f;
//     actor0->angle = 0;
   //actor0->step = step_rotate;

//       actor1 = ks_actor_new("a0002.png", (ks_node_t*)me);
//       actor1->x = Win_W/2;
//       actor1->y = Win_H/2;
//   
//      actor2 = ks_actor_new("kk.png", (ks_node_t*)me);
//      actor2->x = 640 + 20;
//      actor2->y = 128;

    ks_node_sort_by_z((ks_node_t*)me);

    return me;
}

static void generate_grid_img()
{
    int w = 256, h = 256;
    int pitch = w * 4;
    char* pixels = calloc(1, pitch * h);
    int i, j;

    for (i = 0; i < w; ++i)
    {
        if (i % 4 == 0)
        {
            for (j = 0; j < h; ++j)
            {
                *((int*)(&pixels[j * pitch + i * 4])) = 0xffffffff;
            }
        }
    }

     for (i = 0; i < h; ++i)
     {
         if (i % 4 == 0)
         {
             memset(&pixels[i * pitch], 255, pitch);
         }
     }

    ks_helper_image_save_ppm("E:\\grid.ppm", pixels, w, h);
}

int main(int argc, char** argv)
{
    ks_director_init("Hello World", Win_W, Win_H, argc, argv);

    //generate_grid_img();
    ks_utest_start();

    ks_director_run(create_scene());
    return 0;
}
