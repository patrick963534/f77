#ifndef __KS_LIST_H__
#define __KS_LIST_H__

#include <ks/defs.h>

typedef struct ks_list_t
{
    struct ks_list_t*   next;
    struct ks_list_t*   prev;
} ks_list_t;

KS_API void ks_list_init(ks_list_t* head);

#endif