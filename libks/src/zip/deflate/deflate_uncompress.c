#include "deflate.c.h"
#include <stdlib.h>
#include <stdio.h>

#define LEAF_MAX 256

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

static void uncompress(const char* src, int sz, char* dst, int dst_sz)
{
    uchar  *p, *pe;
    uchar   b;
    int     ibit        = 3;
    int     ibyte       = 0;
    int     code        = 0;
    int     code_bits   = 0;
    int     length      = 0;

    pe = (uchar*)dst + dst_sz;
    p  = (uchar*)dst;
    b  = src[ibyte];

    while (p < pe && ibyte < sz)
    {
        code += (((b >> ibit) & 0x1) << code_bits);
        code_bits++;

        if (length == 0)
        {
            if (static_huffman_cache->nbit_code_to_val[code] == code_bits)
            {
                *p++ = (uchar)static_huffman_cache->code_to_val[code];
                code = 0;
                code_bits = 0;
            }
            else if (length_cache->nbit_code_to_val[code] == code_bits)
            {
                length = length_cache->code_to_val[code];
                code = 0;
                code_bits = 0;
            }
        }
        else
        {
            if (offset_cache->nbit_code_to_val[code] == code_bits)
            {
                int offset = offset_cache->code_to_val[code];
                int i, j;
                code = 0;
                code_bits = 0;

                for (i = 0, j = 0; i < length; i++)
                {
                    p[i] = *(p - offset + j);
                    if (++j == offset)
                        j = 0;
                }                

                p += length;

                length = 0;
            }
        }

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