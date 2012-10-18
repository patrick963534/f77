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
    int     (*pop)(ks_event_t* e);
} ks_sys_eventq_interface_t;

#define ks_extends_sys_eventq() \
    ks_extends_object();    \
    const ks_sys_eventq_interface_t* klass

typedef struct ks_sys_eventq_t
{
    ks_extends_sys_eventq();
} ks_sys_eventq_t;

ks_sys_eventq_interface_t* ks_sys_eventq_interface_instance();

KS_API void ks_eventq_init(ks_container_t* container);
KS_API ks_sys_eventq_t* ks_eventq_instance();

KS_API void ks_eventq_endqueue(const ks_event_t* e);
KS_API int  ks_eventq_pop(ks_event_t* e);

#endif
