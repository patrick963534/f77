#include "hufman.c.h"
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>
#include "ks/helper.h"

void build_dynamic_tree(hufman_t* hm)
{
    node_t** nodes = hm->nodes;
    int      sz    = hm->nleaf;
    int      cur   = 0;

    while (sz > 1)
    {
        node_t* n = calloc(1, sizeof(*n));
        node_t* left = nodes[cur];
        node_t* right = nodes[cur + 1];

        n->left = left;
        n->right = right;

        left->parent = n;
        left->bit = 0;

        right->parent = n;
        right->bit = 1;

        n->pt = n->left->pt + n->right->pt;

        nodes[cur + sz] = n;
        cur += 2;
        sz--;

        qsort(nodes + cur, sz, sizeof(nodes[0]), node_comparer);
    }

    hm->root = nodes[cur];
}