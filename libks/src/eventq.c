#include <ks/defs.h>
#include <ks/sys/eventq.h>
#include <ks/list.h>
#include <stdlib.h>

KS_API void ks_eventq_endqueue(const ks_event_t* e)
{
    ks_eventq_instance()->klass->endqueue(e);
}

KS_API int ks_eventq_pop(ks_event_t* e)
{
    return ks_eventq_instance()->klass->pop(e);
}
