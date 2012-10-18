#ifndef __KS_SYS_EVENTQ_H__
#define __KS_SYS_EVENTQ_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/container.h>
#include <ks/event.h>

typedef struct ks_sys_eventq_interface_t
{
    int     nothing;
    void    (*endqueue)(const ks_event_t* e);
    int     (*pop_message)(ks_event_t* e);
} ks_sys_eventq_interface_t;

#define ks_extends_sys_eventq() \
    ks_extends_object();    \
    ks_sys_eventq_interface_t* klass

typedef struct ks_sys_eventq_t
{
    ks_extends_sys_eventq();
} ks_sys_eventq_t;

KS_API void             ks_sys_eventq_init(ks_container_t* container);
KS_API ks_sys_eventq_t* ks_sys_eventq_instance();

#endif
