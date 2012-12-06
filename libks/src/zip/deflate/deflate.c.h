#ifndef __KS_ZIP_DEFLATE_C_H__
#define __KS_ZIP_DEFLATE_C_H__

#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

typedef struct cache_t
{
    int     ncode;
    int*    code_bits;
    int*    code_nbit;

    int*    code_to_val;
    uchar*  nbit_code_to_val;
    int     num_code_to_val;
} cache_t;

cache_t* static_huffman_cache;
cache_t* length_cache;
cache_t* offset_cache;

cache_t* deflate_generate_cache(int ncode, int num_code_to_val, void (*get_code)(unsigned int, int*, int*));

void deflate_init_all_cache();
void deflate_destroy_all_cache();

void deflate_init_static_huffman_cache();
void deflate_init_length_cache();
void deflate_init_offset_cache();

uint revbits(uint n, int numbits);

#endif