#include "hufman.c.h"
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>
#include "ks/helper.h"

int node_comparer(const void* v1, const void* v2)
{
    node_t* n1 = *((node_t**)v1);
    node_t* n2 = *((node_t**)v2);

    return n1->pt - n2->pt;
}

void build_tree(hufman_t* hm)
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

void deep_search_build_codes(hufman_t* hm, const node_t* n, int level)
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

void compression_data_save(compression_data_t* cd)
{
    unsigned char* pt_leaf_count;
    unsigned char* pt_leaf_data;
    unsigned char* pt_uncompress_size;

    int header_sz;
    int content_sz;
    int i;

    header_sz   = sizeof(int) + cd->nleaf * 5 + sizeof(int);
    content_sz  = (cd->bits_count + 8 - 1) / 8;
    
    cd->all = malloc(header_sz + content_sz);

    ks_helper_int_to_bytes((char*)&cd->all[0], cd->nleaf);
 
    pt_leaf_count       = &cd->all[0];
    pt_leaf_data        = &cd->all[4];
    pt_uncompress_size  = &cd->all[4 + cd->nleaf * 5];

    ks_helper_int_to_bytes((char*)&pt_leaf_count, cd->nleaf);

    for (i = 0; i < cd->nleaf; i++)
    {
        pt_leaf_data[i * 5] = cd->leaf_data[i].ch;
        ks_helper_int_to_bytes((char*)&pt_leaf_data[i * 5 + 1], cd->leaf_data[i].pt);
    }

    ks_helper_int_to_bytes((char*)pt_uncompress_size, cd->uncompress_bytes_count);

    cd->content = &cd->all[header_sz];
}

void compression_data_load(compression_data_t* cd, const char* data)
{
    unsigned char* pt_leaf_data;
    unsigned char* pt_uncompress_size;
    int header_sz;
    int i;

    ks_helper_bytes_to_int(data, &cd->nleaf);

    cd->all             = (unsigned char*)data;
    pt_leaf_data        = &cd->all[4];
    pt_uncompress_size  = &cd->all[4 + cd->nleaf * 5];

    for (i = 0; i < cd->nleaf; i++)
    {
        cd->leaf_data[i].ch = pt_leaf_data[i * 5];
        ks_helper_bytes_to_int((char*)&pt_leaf_data[i * 5 + 1], &cd->leaf_data[i].pt);
    }

    ks_helper_bytes_to_int((char*)pt_uncompress_size, &cd->uncompress_bytes_count);
}