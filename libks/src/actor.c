#include <ks/actor.h>
#include <ks/graphics.h>
#include <ks/log.h>

static void draw(ks_actor_t* me)
{
    ks_graphics_draw(me->image, 0, 0, 0, 0, me->image->width, me->image->height);
}

KS_API ks_actor_t* ks_actor_new(const char* conf, int sz, ks_node_t* parent)
{
    ks_actor_t* me;

    me              = (ks_actor_t*)ks_node_new(ks_max(sz, sizeof(*me)), parent);
    me->draw        = (ks_draw_f)draw;
    me->destruct    = (ks_destruct_f)ks_actor_destruct;
    me->tname       = "ks_actor";
    me->image       = ks_image_load(conf, (ks_object_t*)me);

    return me;
}

KS_API void ks_actor_destruct(ks_actor_t* me)
{
    ks_node_destruct((ks_node_t*)me);
}

