#include "hufman.h"
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>

#define LEAF_MAX    256
#define NODE_MAX    512

typedef struct node_t
{
    int             pt;
    unsigned char   ch;
    unsigned char   bit;

    struct node_t* parent;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct code_t
{
    unsigned char   ch;
    unsigned char*  bits;
    int             nbit;
} code_t;

typedef struct hufman_t
{
    const node_t*   root;
    node_t*         nodes[NODE_MAX];

    code_t*         codes[LEAF_MAX];
    unsigned char   leafs[LEAF_MAX];
    int             dict[LEAF_MAX];
    int             nleaf;

    unsigned char*  compress_data;
    int             compress_content_sz;
    int             compress_header_sz;
    int             compress_data_sz;

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
        for (i = code->nbit - 1; i >= 0; i--, np = np->parent)
            code->bits[i] = np->bit;

        hm->codes[n->ch] = code;

        if (level > hm->max_level)
            hm->max_level = level;

        return;
    }

    if (n->left != NULL)
        deep_search_build_codes(hm, n->left, level + 1);

    if (n->right != NULL)
        deep_search_build_codes(hm, n->right, level + 1);
}

static int node_comparer(const void* v1, const void* v2)
{
    node_t* n1 = *((node_t**)v1);
    node_t* n2 = *((node_t**)v2);
    
    return n1->pt - n2->pt;
}

static void com_generate_leafs(hufman_t* hm, const unsigned char* data, int sz)
{
    int       i;
    int*      dict;

    dict = hm->dict;
    for (i = 0; i < sz; i++)
        dict[data[i]] += 1;

    for (i = 0; i < LEAF_MAX; i++)
    {
        if (dict[i] != 0)
        {
            hm->nodes[hm->nleaf] = calloc(1, sizeof(*hm->nodes[0]));
            hm->nodes[hm->nleaf]->left = (void*)0;
            hm->nodes[hm->nleaf]->right = (void*)0;
            hm->nodes[hm->nleaf]->pt = dict[i];
            hm->nodes[hm->nleaf]->ch = (unsigned char)i;
            hm->nleaf++;
        }
    }

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);

    for (i = 0; i < hm->nleaf; i++)
        hm->leafs[i] = hm->nodes[i]->ch;

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

static int calculate_total_bits(hufman_t* hm)
{
    int total = 0;
    int i;
    
    for (i = 0; i < hm->nleaf; i++)
    {
        unsigned int idx = (unsigned char)hm->leafs[i];
        int sz  = hm->codes[idx]->nbit * hm->dict[idx];
        total += sz;
    }

    return total;
}

static void generate_compress_header(hufman_t* hm, int sz)
{
    int* val;
    int  nbit;
    int  i;

    nbit  = calculate_total_bits(hm);

    hm->compress_header_sz = hm->nleaf + sizeof(int);
    hm->compress_content_sz = (nbit + 8 - 1) / 8;
    hm->compress_data_sz = hm->compress_header_sz + hm->compress_content_sz;
    hm->compress_data = (unsigned char*)calloc(1, hm->compress_data_sz);

    for (i = 0; i < hm->nleaf; i++)
        hm->compress_data[i] = hm->leafs[i];

    val = (int*)&hm->compress_data[hm->nleaf];
    *val = sz;
}

static void generate_compress_data(hufman_t* hm, const unsigned char* data, int sz)
{
    unsigned char*  bytes;
    int             i, j;
    int             ibyte;
    int             ibit;

    generate_compress_header(hm, sz);

    bytes = &hm->compress_data[hm->compress_header_sz];
    ibyte = 0;
    ibit  = 0;
    for (i = 0; i < sz; i++)
    {
        unsigned char* b  = &bytes[ibyte];
        unsigned char* cb = hm->codes[data[i]]->bits;
        int   cc = hm->codes[data[i]]->nbit;
        
        for (j = 0; j < cc; j++)
        {
            if (cb[j])
                *b += 1 << ibit;

            if (ibit++ == 8)
            {
                ibit = 0;
                ibyte++;
                b = &bytes[ibyte];
            }
        }
    }
}

char* ks_zip_hufman_compress(const char* data, int sz, int* ret_sz)
{
    hufman_t*       hm;
    unsigned char*  udata;

    hm = calloc(1, sizeof(*hm));
    udata = (unsigned char*)data;

    com_generate_leafs(hm, udata, sz);
    build_tree(hm);
    deep_search_build_codes(hm, hm->root, 0);
    generate_compress_data(hm, udata, sz);

    *ret_sz = hm->compress_data_sz;

    return (char*)hm->compress_data;
}

char* ks_zip_hufman_uncompress(const char* data, int sz, int* ret_sz)
{
    ks_unused(data);
    ks_unused(sz);
    ks_unused(ret_sz);
    return 0;
}
