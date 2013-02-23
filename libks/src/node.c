#include <ks/node.h>

KS_API void ks_node_destruct(ks_node_t* me)
{
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

KS_API void ks_node_add(ks_node_t* me, ks_node_t* o)
{
    ks_list_add_tail(&me->node_children, &o->node_sibling);
}

KS_API void ks_node_remove(ks_node_t* o)
{
    ks_list_remove_init(&o->node_sibling);
}
