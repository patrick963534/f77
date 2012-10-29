#include "hufman.h"
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>

#define LEAF_MAX    256
#define NODE_MAX    512

typedef struct node_t
{
    int     pt;
    char    ch;

    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct code_t
{
    char    val;
    int     bits;
    int     nbit;
} code_t;

typedef struct hufman_t
{
    const node_t*   root;
    node_t*         nodes[NODE_MAX];

    char            leafs[LEAF_MAX];
    int             nleaf;
} hufman_t;

static int node_comparer(const void* v1, const void* v2)
{
    node_t* n1 = *((node_t**)v1);
    node_t* n2 = *((node_t**)v2);
    
    return n1->pt - n2->pt;
}

static void com_generate_leafs(hufman_t* hm, const char* data, int sz)
{
    int       i;
    int       dict[256];

    memset(dict, 0, sizeof(dict));
    for (i = 0; i < sz; i++)
        dict[(unsigned int)data[i]] += 1;

    for (i = 0; i < LEAF_MAX; i++)
    {
        if (dict[i] != 0)
        {
            hm->nodes[hm->nleaf] = calloc(1, sizeof(*hm->nodes[0]));
            hm->nodes[hm->nleaf]->left = (void*)0;
            hm->nodes[hm->nleaf]->right = (void*)0;
            hm->nodes[hm->nleaf]->pt = dict[i];
            hm->nodes[hm->nleaf]->ch = (char)i;
            hm->nleaf++;
        }
    }

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);

    for (i = 0; i < hm->nleaf; i++)
        hm->leafs[hm->nleaf] = hm->nodes[i]->ch;

    for (i = 0; i < hm->nleaf; i++)
        ks_log("0x%02x : %d", hm->nodes[i]->ch, hm->nodes[i]->pt);
}

static void build_tree(hufman_t* hm)
{
    node_t** nodes = hm->nodes;
    int      sz    = hm->nleaf;
    int      cur   = 0;

    while (sz > 1)
    {
        node_t* n = calloc(1, sizeof(*n));

        n->left = nodes[cur];
        n->right = nodes[cur + 1];
        n->pt = n->left->pt + n->right->pt;

        nodes[cur + sz] = n;
        cur += 2;
        sz--;

        qsort(nodes + cur, sz, sizeof(nodes[0]), node_comparer);
    }

    hm->root = nodes[cur];
}

char* ks_zip_hufman_compress(const char* data, int sz, int* ret_sz)
{
    hufman_t* hm;

    hm = calloc(1, sizeof(*hm));

    com_generate_leafs(hm, data, sz);    
    build_tree(hm);

    ks_unused(data);
    ks_unused(sz);
    ks_unused(ret_sz);
    return 0;
}

char* ks_zip_hufman_uncompress(const char* data, int sz, int* ret_sz)
{
    ks_unused(data);
    ks_unused(sz);
    ks_unused(ret_sz);
    return 0;
}
