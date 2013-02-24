#include <ks/node.h>
#include <ks/graphics.h>
#include <stdlib.h>
#include "node.c.h"

static int node_comparer(const void* v1, const void* v2)
{
     ks_node_t* n1 = *((ks_node_t**)v1);
     ks_node_t* n2 = *((ks_node_t**)v2);

     return -(n1->z - n2->z);
}

static ks_node_t** sort_children(ks_node_t* node, int* count)
{
    ks_node_t** children;
    ks_node_t *pos, *n;
    int sz = 0;
    int i = 0;

    ks_node_for_each(pos, n, node, ks_node_t)
    {
        sz++;
    }

    if (sz == 0)
        return NULL;

    children = (ks_node_t**)malloc(sizeof(children[0]) * sz);
    ks_node_for_each(pos, n, node, ks_node_t)
    {
        children[i++] = pos;
    }

    qsort(children, sz, sizeof(children[0]), node_comparer);

    return children;
}

void so_node_draw(ks_node_t* me)
{
    ks_node_t** children;
    int count, i;

    ks_graphics_push();
    ks_graphics_translate(me->x, me->y);

    children = sort_children(me, &count);
    for (i = 0; i < count; ++i)
        so_node_draw(children[i]);

    if (me->draw)
        me->draw(me);

    ks_graphics_pop();

    free(children);
}

void so_node_step(ks_node_t* me, int delta)
{

}

void so_node_msgs(ks_node_t* me, ks_event_t* e)
{

}

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

KS_API void ks_node_add(ks_node_t* me, ks_node_t* child)
{
    ks_list_add_tail(&me->node_children, &child->node_sibling);
    child->parent = me;
}

KS_API void ks_node_remove(ks_node_t* me)
{
    ks_list_remove_init(&me->node_sibling);
    me->parent = NULL;
}


