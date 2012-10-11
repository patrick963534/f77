#ifndef __KS_SYS_SYSTEM_H__
#define __KS_SYS_SYSTEM_H__

#include <ks/defs.h>

#define ks_extends_sys_system()    \
    void    (init*)();          \
    void    (create_window*)(const char* title, int w, int h)

typedef struct ks_sys_system_t
{
    ks_extends_sys_system();
} ks_sys_system_t;

ks_sys_system_t* ks_sys_system_instance();

#endif