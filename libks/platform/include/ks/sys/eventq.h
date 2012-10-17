#ifndef __KS_SYS_EVENTQ_H__
#define __KS_SYS_EVENTQ_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/container.h>

typedef struct ks_sys_eventq_t ks_sys_eventq_t;

typedef struct ks_sys_eventq_interface_t
{
    int     (*add_event)(const char* title, int w, int h);
} ks_sys_eventq_interface_t;

#define ks_extends_sys_eventq() \
    ks_extends_object();    \
    ks_sys_system_interface_t* klass

struct ks_sys_eventq_t
{
    ks_extends_sys_eventq();
};

KS_API void             ks_sys_eventq_init(ks_container_t* container);
KS_API ks_sys_system_t* ks_sys_eventq_instance();

#endif
