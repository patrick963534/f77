#include <ks/container.h>
#include <ks/list.h>
#include <stdlib.h>

KS_API ks_container_t* ks_container_new(int sz)
{
    ks_container_t* me;
    
    me              = (ks_container_t*)calloc(1, ks_max(sz, sizeof(*me)));
    me->destruct    = (ks_destruct_f)ks_container_destruct;
    me->tname       = "ks_container";

    ks_list_init(&me->elements);

    return me;
}

KS_API void ks_container_destruct(ks_container_t* me)
{
    ks_container_delete_elements(me);
    ks_object_destruct((ks_object_t*)me);
}

KS_API void ks_container_delete_elements(ks_container_t* me)
{
    ks_object_t *pos, *n;

    ks_container_for_each_element_revers_safe(pos, n, me, ks_object_t)
    {
        ks_container_remove(pos);
        ks_object_delete(pos);
    }
}

KS_API void ks_container_add(ks_container_t* me, ks_object_t* o)
{
    ks_list_add_tail(&me->elements, &o->element);
}

KS_API void ks_container_remove(ks_object_t* o)
{
    ks_list_remove_init(&o->element);
}
