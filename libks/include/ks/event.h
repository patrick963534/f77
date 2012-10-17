#ifndef __KS_EVENT_H__
#define __KS_EVENT_H__

#include <ks/defs.h>

typedef struct ks_event_key_t
{
    int character;
    int code;
} ks_event_key_t;

typedef struct ks_event_t
{
    int type;
    ks_event_key_t key;
} ks_event_t;

#endif
