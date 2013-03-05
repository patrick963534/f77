#include <ks/node.h>
#include <ks/graphics.h>
#include <ks/log.h>
#include <stdlib.h>

static int node_comparer(const void* v1, const void* v2)
{
    ks_node_t* n1 = *((ks_node_t**)v1);
    ks_node_t* n2 = *((ks_node_t**)v2);

    return -(n1->z - n2->z);
}

static void sort_children_recursive(ks_node_t* node)
{
    #define Sort_Nodes_Cache_Size    1024
    static ks_node_t* sort_nodes_cache[Sort_Nodes_Cache_Size];

    ks_node_t *pos, *n;
    ks_node_t **children = sort_nodes_cache;
    int sz = ks_node_child_count(node);
    int is_heap = 0;
    int i = 0;

    if (sz == 0)
        return;

    if (sz >= 1024)
    {
        is_heap = 1;
        children = (ks_node_t**)malloc(sizeof(children[0]) * sz);
    }

    ks_node_for_each(pos, n, node, ks_node_t)
    {
        ks_node_remove(pos);
        children[i++] = pos;
    }

    qsort(children, sz, sizeof(children[0]), node_comparer);

    for (i = 0; i < sz; i++)
        ks_node_add(node, children[i]);

    if (is_heap)
        free(children);

    ks_node_for_each(pos, n, node, ks_node_t)
    {
        sort_children_recursive(pos);
    }
}

KS_API void ks_node_sort_by_z(ks_node_t* me)
{
    sort_children_recursive(me);
}

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

KS_API int ks_node_child_count(ks_node_t* me)
{
    ks_node_t *pos, *n;
    int sz = 0;

    ks_node_for_each(pos, n, me, ks_node_t)
    {
        sz++;
    }
    
    return sz;
}
