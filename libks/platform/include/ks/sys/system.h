#ifndef __KS_SYS_SYSTEM_H__
#define __KS_SYS_SYSTEM_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/container.h>

typedef struct ks_sys_system_t ks_sys_system_t;

typedef struct ks_sys_system_interface_t
{
    void    (*update_messages)();
    void    (*flush)();
} ks_sys_system_interface_t;

#define ks_extends_sys_system() \
    ks_extends_object();    \
    const ks_sys_system_interface_t* klass

struct ks_sys_system_t
{
    ks_extends_sys_system();
};

KS_API void ks_system_init(ks_container_t* container);
KS_API ks_sys_system_t* ks_system_instance();

KS_API void ks_system_flush();
KS_API void ks_system_update_message();

#endif
