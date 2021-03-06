#include "hufman.c.h"
#include <ks/log.h>
#include <ks/helper.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char* reader_head(hufman_t* hm, const unsigned char* data)
{
    int i;
    compression_data_t cd;

    compression_data_load(&cd, (char*)data);

    hm->nleaf = cd.nleaf;

    for (i = 0; i < cd.nleaf; i++)
    {
        hm->nodes[i]        = calloc(1, sizeof(*hm->nodes[0]));
        hm->nodes[i]->left  = (void*)0;
        hm->nodes[i]->right = (void*)0;
        hm->nodes[i]->ch    = cd.leaf_data[i].ch;
        hm->nodes[i]->pt    = cd.leaf_data[i].pt;

        hm->dict[hm->nodes[i]->ch] = hm->nodes[i]->pt;
    }

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);

    for (i = 0; i < hm->nleaf; i++)
        hm->leafs[i] = hm->nodes[i]->ch;

    hm->uncompress_content_sz = cd.uncompress_bytes_count;

    return cd.content;
}

static unsigned char* uncompress(hufman_t* hm, const unsigned char* content, int sz, int* ret_sz)
{
    unsigned char* udata;
    unsigned char* p;
    unsigned char  b;
    const node_t*  n;
    int i;
    int ibit;
    int ibyte;

    udata = p = (unsigned char*)malloc(hm->uncompress_content_sz);
    ibit  = 0;
    ibyte = 0;
    i     = 0;
    n     = hm->root;
    b     = content[ibyte];

    while (i < hm->uncompress_content_sz)
    {
        if (n->left == NULL && n->right == NULL)
        {
            i++;
            *p++ = n->ch;
            n = hm->root;
            continue;
        }

        if (((b >> (7 - ibit)) & 0x1) == 0)
            n = n->left;
        else
            n = n->right;

        if (++ibit == 8)
        {
            ibit = 0;
            ibyte++;
            b = content[ibyte];
        }
    }

    *ret_sz = hm->uncompress_content_sz;

    ks_unused(sz);
    return udata;
}

char* zip_hufman_uncompress(const char* data, int sz, int* ret_sz)
{
    hufman_t*       hm;
    unsigned char*  udata;

    hm = calloc(1, sizeof(*hm));

    udata = reader_head(hm, (unsigned char*)data);
    build_dynamic_tree(hm);
    deep_search_build_codes(hm, hm->root, 0);
    udata = uncompress(hm, udata, sz, ret_sz);
    delete_hm(hm);

    return (char*)udata;
}
