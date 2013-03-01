#include <ks/node.h>
#include <ks/graphics.h>
#include <ks/log.h>
#include <stdlib.h>

KS_API void ks_node_destruct(ks_node_t* me)
{
    ks_node_delete_children(me);
    ks_object_destruct((ks_object_t*)me);
}

KS_API ks_node_t* ks_node_new(int sz, ks_node_t* parent)
{
    ks_node_t* me;

    me              = (ks_node_t*)ks_object_new(ks_max(sz, sizeof(*me)));
    me->destruct    = (ks_destruct_f)ks_object_destruct;
    me->tname       = "ks_node";

    ks_list_init(&me->node_children);
    ks_list_init(&me->node_sibling);

    if (parent)
        ks_node_add(parent, me);

    return me;
}

KS_API void ks_node_delete_children(ks_node_t* me)
{
    ks_node_t *pos, *n;

    ks_node_for_each(pos, n, me, ks_node_t)
    {
        ks_node_remove(pos);
        ks_object_delete(pos);
    }
}

KS_API void ks_node_add(ks_node_t* me, ks_node_t* child)
{
    ks_list_add_tail(&me->node_children, &child->node_sibling);
    ks_object_remove((ks_object_t*)child);

    child->parent = me;
}

KS_API void ks_node_remove(ks_node_t* me)
{
    ks_list_remove_init(&me->node_sibling);
    me->parent = NULL;
}

KS_API int ks_node_has_child(ks_node_t* me)
{
    return !ks_list_empty(&me->node_children);
}
