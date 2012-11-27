#ifndef __KS_ZIP_HUFMAN_C_H__
#define __KS_ZIP_HUFMAN_C_H__

#include <ks/defs.h>
#include "hufman.h"

#define LEAF_MAX    256
#define NODE_MAX    1024

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

    int             compress_content_sz;
    int             compress_header_sz;
    int             compress_data_sz;

    int             uncompress_content_sz;
    int             max_level;
} hufman_t;

typedef struct data_leaf_t
{
    unsigned char   ch;
    int             pt;
} data_leaf_t;

typedef struct compression_data_t
{
    int             nleaf;
    data_leaf_t     leaf_data[LEAF_MAX];
    int             uncompress_bytes_count;
    int             compress_bytes_count;
    int             bits_count;
    unsigned char*  all;
    unsigned char*  content;
} compression_data_t;

int     node_comparer(const void* v1, const void* v2);
void    build_dynamic_tree(hufman_t* hm);
void    build_static_tree(hufman_t* hm);
void    deep_search_build_codes(hufman_t* hm, const node_t* n, int level);
void    delete_hm(hufman_t* hm);

void    compression_data_save(compression_data_t* cd);
void    compression_data_load(compression_data_t* cd, const char* data);

#endif