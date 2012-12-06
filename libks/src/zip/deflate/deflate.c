#include "deflate.c.h"
#include <stdlib.h>

uint revbits(uint n, int numbits)
{
    static const uchar rev5[32] = {
        0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 
        10, 26, 6, 22, 14, 30, 1, 17, 9, 25, 
        5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31
    };

    uint i, result = 0;
    if(numbits == 5)
        return rev5[n];
    else
    {
        for (i = numbits; i; --i)
        {
            result <<= 1;
            if (n & 1)
                result |= 1;
            n >>= 1;
        }
        return result;
    }
}

cache_t* deflate_generate_cache(int ncode, int num_code_to_val, void (*get_code)(unsigned int, int*, int*))
{
    cache_t* me;
    int i, max_code;

    me = calloc(1, sizeof(*me));

    me->ncode = ncode;
    me->code_bits = calloc(me->ncode, sizeof(me->code_bits[0]));
    me->code_nbit = calloc(me->ncode, sizeof(me->code_nbit[0]));

    me->num_code_to_val  = num_code_to_val;
    me->nbit_code_to_val = calloc(me->num_code_to_val, sizeof(me->nbit_code_to_val[0]));
    me->code_to_val      = calloc(me->num_code_to_val, sizeof(me->code_to_val[0]));

    max_code = 0;
    for (i = 0; i < me->ncode; i++)
    {
        get_code(i, &me->code_bits[i], &me->code_nbit[i]);
        me->code_to_val[me->code_bits[i]] = i;
        me->nbit_code_to_val[me->code_bits[i]] = (uchar)me->code_nbit[i];

        if (max_code < me->code_bits[i])
            max_code = me->code_bits[i];
    }

    if (max_code < me->num_code_to_val - 1)
    {
        me->num_code_to_val = max_code;
        me->code_to_val = realloc(me->code_to_val, me->num_code_to_val * sizeof(me->code_to_val[0]));
    }

    return me;
}

void deflate_init_all_cache()
{
    deflate_init_static_huffman_cache();
    deflate_init_length_cache();
    deflate_init_offset_cache();
}

void deflate_destroy_all_cache()
{
#define DESTROY_CACHE(p) { free(p->code_bits); free(p->code_nbit); free(p); p = NULL; }
    DESTROY_CACHE(static_huffman_cache);
    DESTROY_CACHE(length_cache);
    DESTROY_CACHE(offset_cache);
#undef DESTROY_CACHE
}