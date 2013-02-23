#include <ks/object.h>
#include <ks/list.h>
#include <stdlib.h>

KS_API void ks_object_destruct(ks_object_t* me)
{
    ks_object_delete_children(me);
    ks_unused(me);
}

KS_API ks_object_t* ks_object_new(int sz)
{
    ks_object_t* me;

    me              = (ks_object_t*)calloc(1, ks_max(sz, sizeof(*me)));
    me->destruct    = (ks_destruct_f)ks_object_destruct;
    me->tname       = "ks_object";
    me->magic       = 88888888;
    me->heap        = 1;

    ks_list_init(&me->object_children);
    ks_list_init(&me->object_sibling);

    return me;
}

KS_API void ks_object_delete_children(ks_object_t* me)
{
    ks_object_t *pos, *n;

    ks_object_for_each(pos, n, me, ks_object_t)
    {
        ks_object_remove(pos);
        ks_object_delete(pos);
    }
}

KS_API void ks_object_add(ks_object_t* me, ks_object_t* o)
{
    ks_list_add_tail(&me->object_children, &o->object_sibling);
}

KS_API void ks_object_remove(ks_object_t* o)
{
    ks_list_remove_init(&o->object_sibling);
}

KS_API void ks_object_delete(void* me_)
{
    ks_cast(ks_object_t);

    if (me->destruct)
        me->destruct(me);

    me->magic = 0;

    if (me->heap)
        free(me);
}
