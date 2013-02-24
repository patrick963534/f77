#ifndef __KS_NODE_C_H__
#define __KS_NODE_C_H__

#include <ks/node.h>
#include <ks/event.h>

void so_node_draw(ks_node_t* me);
void so_node_step(ks_node_t* me, int delta);
void so_node_msgs(ks_node_t* me, ks_event_t* e);

#endif
