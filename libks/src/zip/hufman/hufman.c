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

void deep_search_build_codes(hufman_t* hm, const node_t* n, int level)
{
    if (n->left == NULL && n->right == NULL)
    {
        code_t* code;
        node_t* np;
        int i;

        code = calloc(1, sizeof(*code));
        code->nbit = level;
        code->ch   = n->ch;
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

void delete_hm(hufman_t* hm)
{
    int i;

    for (i = 0; i < NODE_MAX; i++)
        free(hm->nodes[i]);

    for (i = 0; i < LEAF_MAX; i++)
        free(hm->codes[i]);

    free(hm);
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
    
    cd->compress_bytes_count = header_sz + content_sz;
    cd->all = malloc(cd->compress_bytes_count);

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

    header_sz   = sizeof(int) + cd->nleaf * 5 + sizeof(int);
    cd->content = &cd->all[header_sz];
}
