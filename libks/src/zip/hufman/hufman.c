#include "hufman.h"
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>

#define LEAF_MAX    256
#define NODE_MAX    512

typedef struct node_t
{
    int     point;
    char    val;

    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct hufman_t
{
    node_t*  nodes[NODE_MAX];
    int      total;
    int      current;
    int      count;

    char     leafs[LEAF_MAX];
    int      nleaf;
} hufman_t;

static int node_comparer(const void* v1, const void* v2)
{
    node_t** vp1 = (node_t**)v1;
    node_t** vp2 = (node_t**)v2;

    node_t* n1 = (node_t*)*vp1;
    node_t* n2 = (node_t*)*vp2;
    
    return n1->point - n2->point;
}

char* ks_zip_hufman_compress(const char* data, int sz, int* ret_sz)
{
    hufman_t* hm;
    int       i, k;
    int       dict[256];

    memset(dict, 0, sizeof(dict));
    for (i = 0; i < sz; i++)
        dict[(unsigned int)data[i]] += 1;

    hm = calloc(1, sizeof(*hm));
    for (i = 0; i < LEAF_MAX; i++)
    {
        if (dict[i] != 0)
        {
            k = hm->nleaf;

            hm->nodes[k] = calloc(1, sizeof(hm->nodes[0]));
            hm->nodes[k]->point = dict[i];
            hm->nodes[k]->val = (char)i;

            hm->leafs[k] = hm->nodes[k]->val;
            hm->nleaf++;

            ks_log("0x%02x : %d", hm->nodes[k]->val, hm->nodes[k]->point);
        }
    }

    ks_log("%s", "*************************");

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);

    for (i = 0; i < hm->nleaf; i++)
        ks_log("0x%02x : %d", hm->nodes[i]->val, hm->nodes[i]->point);
    
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