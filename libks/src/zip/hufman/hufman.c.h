#ifndef __KS_ZIP_HUFMAN_C_H__
#define __KS_ZIP_HUFMAN_C_H__

#include <ks/defs.h>
#include "hufman.h"

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

    int             uncompress_content_sz;
    int             max_level;
} hufman_t;

int     node_comparer(const void* v1, const void* v2);
void    build_tree(hufman_t* hm);
void    deep_search_build_codes(hufman_t* hm, const node_t* n, int level);

#endif