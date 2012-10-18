#include <ks/defs.h>
#include <ks/sys/eventq.h>
#include <ks/list.h>
#include <stdlib.h>

typedef struct eventq_t
{
    ks_extends_sys_eventq();

    ks_list_t events;
} eventq_t;

typedef struct event_node_t
{
    ks_list_t   element;
    ks_event_t  ev;
} event_node_t;

static eventq_t* eventq;

static void destruct(eventq_t* me)
{
    event_node_t *pos, *n;

    ks_list_for_each_entry_reverse_safe(pos, n, &me->events, event_node_t, element)
    {
        ks_list_remove(&pos->element);
        free(pos);
    }

    ks_object_destruct((ks_object_t*)me);
}

static void endqueue(const ks_event_t* e)
{
    event_node_t* node = malloc(sizeof(*node));
    node->ev = *e;
    ks_list_init(&node->element);

    ks_list_add_tail(&eventq->events, &node->element);
}

static int pop(ks_event_t* e)
{
    event_node_t* node;

    if (ks_list_empty(&eventq->events))
        return 0;

    node = ks_list_first_entry(&eventq->events, event_node_t, element);
    *e = node->ev;

    ks_list_remove(&node->element);
    free(node);

    return 1;
}

KS_API void ks_eventq_endqueue(const ks_event_t* e)
{
    endqueue(e);
}

KS_API int ks_eventq_pop(ks_event_t* e)
{
    return pop(e);
}

KS_API void ks_eventq_init(ks_container_t* container)
{
    eventq             = (eventq_t*)ks_object_new(sizeof(*eventq));
    eventq->destruct   = (ks_destruct_f)destruct;
    eventq->klass      = ks_sys_eventq_interface_instance();

    ks_list_init(&eventq->events);

    if (container)
        ks_container_add(container, (ks_object_t*)eventq);
}

KS_API ks_sys_eventq_t* ks_sys_eventq_instance()
{
    return (ks_sys_eventq_t*)eventq;
}
