#include <ks/scene.h>
#include <ks/list.h>
#include <stdlib.h>
#include <ks/object.h>
#include <ks/node.h>
#include <ks/event.h>

typedef struct z_node_t
{
    ks_list_t   list;
    ks_node_t*  node;
} z_node_t;

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
