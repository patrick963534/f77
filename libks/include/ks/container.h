#ifndef __KS_CONTAINER_H__
#define __KS_CONTAINER_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/list.h>

#define ks_extends_container()  \
    ks_extends_object();        \
    ks_list_t elements

typedef struct ks_container_t
{
    ks_extends_container();
} ks_container_t;

KS_API ks_container_t*  ks_container_new(int sz);
KS_API void             ks_container_destruct(ks_container_t* me);
KS_API void             ks_container_delete_elements(ks_container_t* me);
KS_API void             ks_container_add(ks_container_t* me, ks_object_t* o);
KS_API void             ks_container_remove(ks_object_t* o);

#define ks_container_for_each_element_safe(pos, n, container, type)    \
    for (pos = ks_list_entry((&container->elements)->next, type, element),           \          n = ks_list_entry(pos->element.next, type, element);        \         &pos->element != (&container->elements);                                    \          pos = n, n = ks_list_entry(n->element.next, type, element))

#define ks_container_for_each_element_revers_safe(pos, n, container, type)    \
    for (pos = ks_list_entry((&container->elements)->prev, type, element),           \          n = ks_list_entry(pos->element.prev, type, element);        \         &pos->element != (&container->elements);                                    \          pos = n, n = ks_list_entry(n->element.prev, type, element))

#endif