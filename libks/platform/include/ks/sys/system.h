#ifndef __KS_SYSTEM_H__
#define __KS_SYSTEM_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/container.h>

typedef struct ks_sys_system_interface_t
{
    void    (*update_messages)();
    void    (*flush)();
} ks_sys_system_interface_t;

#define ks_extends_system() \
    ks_extends_object();    \
    const ks_sys_system_interface_t* klass

typedef struct ks_system_t
{
    ks_extends_system();
} ks_system_t;

KS_API void ks_system_init(ks_container_t* container);
KS_API ks_system_t* ks_system_instance();

KS_API void ks_system_flush();
KS_API void ks_system_update_message();

#endif
