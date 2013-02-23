#ifndef __KS_NODE_H__
#define __KS_NODE_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/event.h>

typedef struct ks_node_t ks_node_t;

typedef void (*ks_draw_f)(ks_node_t* me_);
typedef void (*ks_step_f)(ks_node_t* me_, int delta);
typedef void (*ks_msgs_f)(ks_node_t* me_, ks_event_t* e);

#define ks_extends_node()           \
    ks_extends_object();            \
    int         x, y, z;            \
    ks_list_t   node_children;      \
    ks_list_t   node_sibling;       \
    ks_step_f   step;               \
    ks_msgs_f   msgs;               \
    ks_draw_f   draw

struct ks_node_t
{
    ks_extends_node();
};

KS_API ks_node_t*   ks_node_new(int sz, ks_node_t* parent);
KS_API void         ks_node_destruct(ks_node_t* me);
KS_API void         ks_node_delete_children(ks_node_t* me);
KS_API void         ks_node_add(ks_node_t* me, ks_node_t* o);
KS_API void         ks_node_remove(ks_node_t* o);

#define ks_node_for_each(pos, n, node, type)                                  \
    for (pos = ks_list_entry(node->node_children.next, type, node_sibling),   \
           n = ks_list_entry(pos->node_sibling.next, type, node_sibling);     \
         &pos->node_sibling != (&node->node_children);                        \
         pos =n, n = ks_list_entry(n->node_sibling.next, type, node_sibling))

#define ks_node_for_each_reverse(pos, n, node, type)                          \
    for (pos = ks_list_entry(node->node_children.prev, type, node_sibling),   \
           n = ks_list_entry(pos->node_sibling.prev, type, node_sibling);     \
         &pos->node_sibling != (&node->node_children);                        \
         pos =n, n = ks_list_entry(n->node_sibling.prev, type, node_sibling))

#endif
