#ifndef __KS_OBJECT_H__
#define __KS_OBJECT_H__

#include <ks/defs.h>
#include <ks/list.h>

typedef void (*ks_destruct_f)(void* o);

#define ks_extends_object()             \
    int             magic;              \
    int             heap:1;             \
    int             unused:31;          \
    char*           tname;              \
    ks_list_t       object_children;    \
    ks_list_t       object_sibling;     \
    ks_destruct_f   destruct

typedef struct ks_object_t
{
    ks_extends_object();
} ks_object_t;

KS_API ks_object_t* ks_object_new(int sz);

KS_API void         ks_object_delete(void* o);
KS_API void         ks_object_destruct(ks_object_t* me_);

KS_API void         ks_object_delete_children(ks_object_t* me);
KS_API void         ks_object_add(ks_object_t* me, ks_object_t* o);
KS_API void         ks_object_remove(ks_object_t* o);

#define ks_object_for_each(pos, n, object, type)    \
    for (pos = ks_list_entry(object->object_children.next, type, object_sibling),   \
           n = ks_list_entry(pos->object_sibling.next, type, object_sibling);          \
         &pos->object_sibling != (&object->object_children);                            \
         pos =n, n = ks_list_entry(n->object_sibling.next, type, object_sibling))

#define ks_object_for_each_reverse(pos, n, object, type)    \
    for (pos = ks_list_entry(object->object_children.prev, type, object_sibling),   \
           n = ks_list_entry(pos->object_sibling.prev, type, object_sibling);          \
         &pos->object_sibling != (&object->object_children);                            \
         pos =n, n = ks_list_entry(n->object_sibling.prev, type, object_sibling))

#endif
