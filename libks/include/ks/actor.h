#ifndef __KS_ACTOR_H__
#define __KS_ACTOR_H__

#include <ks/defs.h>
#include <ks/node.h>
#include <ks/image.h>

#define ks_extends_actor()           \
    ks_extends_node();            \
    ks_image_t* image

typedef struct ks_actor_t
{
    ks_extends_actor();
} ks_actor_t;

KS_API ks_actor_t*  ks_actor_new(const char* conf, int sz, ks_node_t* parent);
KS_API void         ks_actor_destruct(ks_actor_t* me);

#endif
