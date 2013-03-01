#ifndef __KS_SCENE_H__
#define __KS_SCENE_H__

#include <ks/defs.h>
#include <ks/node.h>

typedef int  (*ks_msgs_f)(ks_node_t* me_, ks_event_t* e);

typedef struct ks_scene_t
{
    ks_extends_node();
    ks_msgs_f   msgs; 

} ks_scene_t;

KS_API ks_scene_t*  ks_scene_new(int size);
KS_API void         ks_scene_destruct(ks_scene_t* me);

#endif
