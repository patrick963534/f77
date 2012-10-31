#include "hufman.c.h"
#include <ks/log.h>
#include <ks/helper.h>
#include <stdlib.h>
#include <string.h>

static void reader_head(hufman_t* hm, const unsigned char* data)
{
    int i;

    ks_helper_bytes_to_int((char*)&data[0], &hm->nleaf);

    for (i = 0; i < hm->nleaf; i++)
    {
        hm->nodes[i]        = calloc(1, sizeof(*hm->nodes[0]));
        hm->nodes[i]->left  = (void*)0;
        hm->nodes[i]->right = (void*)0;
        hm->nodes[i]->ch    = (unsigned char)data[4 + i * 5];

        ks_helper_bytes_to_int((char*)&data[4 + i * 5 + 1], &hm->nodes[i]->pt);
        hm->dict[hm->nodes[i]->ch] = hm->nodes[i]->pt;
    }

    qsort(hm->nodes, hm->nleaf, sizeof(hm->nodes[0]), node_comparer);

    for (i = 0; i < hm->nleaf; i++)
        hm->leafs[i] = hm->nodes[i]->ch;

    ks_helper_bytes_to_int((char*)&data[4 + hm->nleaf], &hm->uncompress_content_sz);

    for (i = 0; i < hm->nleaf; i++)
        ks_log("0x%02x : %d", hm->nodes[i]->ch, hm->nodes[i]->pt);
}

char* ks_zip_hufman_uncompress(const char* data, int sz, int* ret_sz)
{
    hufman_t*       hm;
    unsigned char*  udata;

    hm = calloc(1, sizeof(*hm));
    udata = (unsigned char*)data;

    reader_head(hm, udata);
    build_tree(hm);
    deep_search_build_codes(hm, hm->root, 0);

    ks_unused(data);
    ks_unused(sz);
    ks_unused(ret_sz);
    return 0;
}
