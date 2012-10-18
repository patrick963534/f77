#include <ks/defs.h>
#include <ks/sys/eventq.h>
#include <ks/list.h>
#include <stdlib.h>

static const ks_sys_eventq_interface_t interfaces = {
    0,
};

const ks_sys_eventq_interface_t* ks_sys_eventq_interface_instance()
{
    return &interfaces;
}
