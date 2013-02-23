#include <ks/scene.h>
#include <ks/list.h>
#include <stdlib.h>
#include "ks/object.h"

KS_API ks_scene_t* ks_scene_new(int sz)
{
    ks_scene_t* me;

    me              = (ks_scene_t*)calloc(1, ks_max(sz, sizeof(*me)));
    me->destruct    = (ks_destruct_f)ks_scene_destruct;
    me->tname       = "ks_scene";

    return me;
}

KS_API void ks_scene_destruct(ks_scene_t* me)
{
    ks_object_destruct((ks_object_t*)me);
}
