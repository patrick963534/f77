#ifndef __KS_OBJECT_H__
#define __KS_OBJECT_H__

#include <ks/defs.h>
#include <ks/list.h>

typedef void (*ks_destruct_f)(void* o);

#define ks_extends_object()     \
    int             magic;      \
    int             heap;       \
    char*           tname;      \
    ks_destruct_f   destruct;   \
    ks_list_t       element     /* Object can be added as children, but can NOT add children. */
    
typedef struct ks_object_t
{
    ks_extends_object();
} ks_object_t;

KS_API void         ks_object_delete(void* o);

KS_API ks_object_t* ks_object_new(int sz);
KS_API void         ks_object_destruct(ks_object_t* me_);

#define ks_extends_container()  \
    ks_extends_object();        \
    ks_list_t elements

typedef struct ks_container_t
{
    ks_extends_container();
} ks_container_t;

KS_API ks_container_t*  ks_container_new(int sz);
KS_API void             ks_container_destruct(ks_object_t* me_);

#endif