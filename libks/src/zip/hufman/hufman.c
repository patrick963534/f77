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

    char    code;

    struct node_t* parent;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct code_t
{
    char    ch;
    char*   bits;
    int     nbit;
} code_t;

typedef struct hufman_t
{
    const node_t*   root;
    node_t*         nodes[NODE_MAX];

    code_t*         codes[LEAF_MAX];
    char            leafs[LEAF_MAX];
    int             nleaf;

    int             max_level;
} hufman_t;

static void deep_search_build_codes(hufman_t* hm, const node_t* n, int level)
{
    if (n->left == NULL && n->right == NULL)
    {
        code_t* code;
        node_t* np;
        int i;

        code = calloc(1, sizeof(*code));
        code->nbit = level + 1;
        code->bits = calloc(code->nbit, sizeof(code->bits[0]));

        np = (node_t*)n;
        for (i = code->nbit - 1; i >= 0; i--)
        {
            code->bits[i] = np->code;
            np = np->parent;
        }

        hm->codes[(unsigned char)n->ch] = code;

        if (level > hm->max_level)
            hm->max_level = level;

        return;
    }

    if (n->left != NULL)
    {
        deep_search_build_codes(hm, n->left, level + 1);
    }

    if (n->right != NULL)
    {
        deep_search_build_codes(hm, n->right, level + 1);
    }
}

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
        dict[(unsigned char)data[i]] += 1;

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
        hm->leafs[i] = hm->nodes[i]->ch;

    for (i = 0; i < hm->nleaf; i++)
        ks_log("0x%02x : %d", (unsigned char)hm->nodes[i]->ch, hm->nodes[i]->pt);
}

static void build_tree(hufman_t* hm)
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
        left->code = 0;

        right->parent = n;
        right->code = 1;

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
    deep_search_build_codes(hm, hm->root, 0);

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
