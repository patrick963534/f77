#include "deflate.c.h"
#include <stdlib.h>
#include <stdio.h>

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

static node_t* build_static_tree()
{
    node_t* root;
    int i, j;

    root = calloc(1, sizeof(*root));

    for (i = 0; i < LEAF_MAX; i++)
    {
        int bits = encoding[i].bits;
        int nbit = encoding[i].nbit;
        node_t* cur = root;

        for (j = 0; j < nbit; j++)
        {
            int v = ((bits >> j) & 0x1);
            if (v)
            {
                if (cur->right == NULL)
                {
                    node_t* n = calloc(1, sizeof(*n));
                    cur->right = n;
                    n->parent = cur;
                    n->bit = 1;
                }
                cur = cur->right;
            }
            else
            {
                if (cur->left == NULL)
                {
                    node_t* n = calloc(1, sizeof(*n));
                    cur->left = n;
                    n->parent = cur;
                    n->bit = 0;
                }
                cur = cur->left;
            }
        }

        cur->ch = (unsigned char)i;
    }

    return root;
}

static void deep_search_build_codes(code_t** codes, const node_t* n, int level)
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

        codes[n->ch] = code;

        return;
    }

    if (n->left != NULL)
        deep_search_build_codes(codes, n->left, level + 1);

    if (n->right != NULL)
        deep_search_build_codes(codes, n->right, level + 1);
}

static void uncompress(const char* src, int sz, char* dst, int dst_sz)
{
    node_t* root;
    node_t* n;
    code_t* codes[LEAF_MAX];
    uchar*  p;
    uchar   b;
    int     i;
    int     ibit = 3;
    int     ibyte = 0;

    for (i = 0; i < LEAF_MAX; i++)
        codes[i] = NULL;

    root = build_static_tree();
    deep_search_build_codes(codes, root, 0);

    p = (uchar*)dst;
    n = root;
    b = src[ibyte];
    i = 0;

    while (i < dst_sz && ibyte < sz)
    {
        if (n->left == NULL && n->right == NULL)
        {
            i++;
            *p++ = n->ch;
            n = root;
            continue;
        }

        if (((b >> ibit) & 0x1) == 0)
            n = n->left;
        else
            n = n->right;

        if (++ibit == 8)
        {
            ibit = 0;
            ibyte++;
            b = src[ibyte];
        }
    }
}

char* zip_deflate_uncompress(const char* data, int sz, int* ret_sz)
{
    int nbyte;
    char* dst;

    nbyte = *ret_sz = *((int*)data);
    dst   = calloc(1, nbyte);
    uncompress(&data[4], sz - 4, dst, nbyte);

    return dst;
}