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
            hm->nleaf++;
        }
    }

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);

    for (i = 0; i < hm->nleaf; i++)
        hm->leafs[i] = hm->nodes[i]->ch;

    for (i = 0; i < hm->nleaf; i++)
        ks_log("0x%02x : %d", hm->nodes[i]->ch, hm->nodes[i]->pt);
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
    unsigned char* pt_leaf_count;
    unsigned char* pt_leaf_data;
    unsigned char* pt_uncompress_size;

    int  nbit;
    int  i;

    nbit = calculate_total_bits(hm);

    hm->compress_header_sz = sizeof(int) + hm->nleaf * 5 + sizeof(int);
    hm->compress_content_sz = (nbit + 8 - 1) / 8;
    hm->compress_data_sz = hm->compress_header_sz + hm->compress_content_sz;
    hm->compress_data = (unsigned char*)calloc(1, hm->compress_data_sz);

    pt_leaf_count       = &hm->compress_data[0];
    pt_leaf_data        = &hm->compress_data[4];
    pt_uncompress_size  = &hm->compress_data[4 + hm->nleaf * 5];

    ks_helper_int_to_bytes((char*)&hm->compress_data[0], hm->nleaf);

    for (i = 0; i < hm->nleaf; i++)
    {
        pt_leaf_data[i * 5] = hm->leafs[i];
        ks_helper_int_to_bytes((char*)&pt_leaf_data[i * 5 + 1], hm->dict[hm->leafs[i]]);
    }

    ks_helper_int_to_bytes((char*)pt_uncompress_size, sz);
}

static void compress_data(hufman_t* hm, const unsigned char* data, int sz)
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

    reader_head(hm, udata, sz);
    build_tree(hm);
    deep_search_build_codes(hm, hm->root, 0);
    compress_data(hm, udata, sz);

    *ret_sz = hm->compress_data_sz;

    return (char*)hm->compress_data;
}