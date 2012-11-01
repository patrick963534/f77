#include "hufman.c.h"
#include <ks/log.h>
#include <ks/helper.h>
#include <stdlib.h>
#include <string.h>


static void reader_head(hufman_t* hm, const unsigned char* data, int sz)
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
            hm->leafs[hm->nleaf]     = (unsigned char)i;
            hm->nleaf++;
        }
    }

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);
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

static void generate_compress_header(compression_data_t* cd, hufman_t* hm, int sz)
{
    int i;

    cd->bits_count = calculate_total_bits(hm);
    cd->nleaf = hm->nleaf;
    cd->uncompress_bytes_count = sz;

    for (i = 0; i < cd->nleaf; i++)
    {
        cd->leaf_data[i].ch = hm->leafs[i];
        cd->leaf_data[i].pt = hm->dict[hm->leafs[i]];
    }

    compression_data_save(cd);
    hm->compress_data_sz = cd->compress_bytes_count;
}

static char* compress_data(hufman_t* hm, const unsigned char* data, int sz, int *ret_sz)
{
    unsigned char*  bytes;
    unsigned char   b;
    int             i, j;
    int             ibyte;
    int             ibit;

    compression_data_t cd;

    generate_compress_header(&cd, hm, sz);

    bytes = cd.content;
    ibyte = 0;
    ibit  = 0;
    b     = 0;
    for (i = 0; i < sz; i++)
    {
        unsigned char* cb = hm->codes[data[i]]->bits;
        int   cc = hm->codes[data[i]]->nbit;
        
        for (j = 0; j < cc; j++)
        {
            if (cb[j])
                b += 1 << (7 - ibit);

            if (++ibit == 8)
            {
                ibit = 0;
                bytes[ibyte] = b;
                ibyte++;
                b = 0;
            }
        }
    }
    bytes[ibyte] = b;

    *ret_sz = hm->compress_data_sz;

    return (char*)cd.all;
}

char* ks_zip_hufman_compress(const char* data, int sz, int* ret_sz)
{
    hufman_t*       hm;
    unsigned char*  udata;
    char*           result;

    hm = calloc(1, sizeof(*hm));
    udata = (unsigned char*)data;

    reader_head(hm, udata, sz);
    build_tree(hm);
    deep_search_build_codes(hm, hm->root, 0);
    result = compress_data(hm, udata, sz, ret_sz);
    delete_hm(hm);

    return result;
}