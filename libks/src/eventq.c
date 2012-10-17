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
    ks_unused(me);

    ks_container_remove((ks_object_t*)me);
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

static ks_sys_eventq_interface_t interfaces = {
    0,
    endqueue,
    pop
};

KS_API void ks_sys_eventq_init(ks_container_t* container)
{
    eventq             = (eventq_t*)ks_object_new(sizeof(*eventq));
    eventq->destruct   = (ks_destruct_f)destruct;
    eventq->klass      = &interfaces;

    ks_list_init(&eventq->events);

    if (container)
        ks_container_add(container, (ks_object_t*)eventq);
}

KS_API ks_sys_eventq_t* ks_sys_eventq_instance()
{
    return (ks_sys_eventq_t*)eventq;
}
