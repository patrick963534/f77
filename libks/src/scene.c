#include <ks/scene.h>
#include <ks/list.h>
#include <stdlib.h>
#include <ks/object.h>
#include <ks/node.h>
#include <ks/event.h>
#include <ks/graphics.h>

static void node_draw(ks_node_t* me)
{
    ks_node_t *pos, *n;

    ks_graphics_push();
    ks_graphics_translate(me->x, me->y, me->z);

    ks_node_for_each(pos, n, me, ks_node_t)
    {
        node_draw(pos);
    }

    if (me->draw)
        me->draw(me);

    ks_graphics_pop();
}

static void node_step(ks_node_t* me, int delta)
{
    ks_node_t *pos, *n;

    ks_node_for_each(pos, n, me, ks_node_t)
    {
        if (ks_node_has_child(pos))
            node_step(pos, delta);

        if (pos->step)
            pos->step(pos, delta);
    }
}

void so_scene_step(ks_scene_t* me, int delta)
{
    node_step((ks_node_t*)me, delta);
}

void so_scene_draw(ks_scene_t* me)
{
    node_draw((ks_node_t*)me);
}

KS_API ks_scene_t* ks_scene_new(int sz)
{
    ks_scene_t* me;

    me              = (ks_scene_t*)ks_node_new(ks_max(sz, sizeof(*me)), NULL);
    me->destruct    = (ks_destruct_f)ks_scene_destruct;
    me->tname       = "ks_scene";

    return me;
}

KS_API void ks_scene_destruct(ks_scene_t* me)
{
    ks_object_destruct((ks_object_t*)me);
}
