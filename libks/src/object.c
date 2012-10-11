#include <ks/object.h>
#include <stdlib.h>

KS_API void ks_object_destruct(ks_object_t* me_)
{
    ks_unused(me_);
}

KS_API ks_object_t* ks_object_new(int sz)
{
    ks_object_t* me;
    
    me              = (ks_object_t*)calloc(1, max(sz, sizeof(*me)));
    me->destruct    = (ks_destruct_f)ks_object_destruct;
    me->tname       = "ks_object";
    me->magic       = 88888888;
    me->heap        = 1;

    ks_list_init(&me->element);

    return me;
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