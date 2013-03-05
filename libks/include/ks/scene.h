#ifndef __KS_SCENE_H__
#define __KS_SCENE_H__

#include <ks/defs.h>
#include <ks/node.h>

typedef struct ks_scene_t
{
    ks_extends_node();

} ks_scene_t;

KS_API ks_scene_t*  ks_scene_new(int size);
KS_API void         ks_scene_destruct(ks_scene_t* me);

#endif
