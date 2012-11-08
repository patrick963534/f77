#include "lz77.c.h"

static int build_header(lz77_t* lz)
{
    pair_t* pos;
    int     bits = 0;
    int     pair_bit_sz = 0;
    int     offset = 0;
    int     length = 0;
    int     i;

    ks_list_for_each_entry(pos, &lz->pairs, pair_t, e)
    {
        if (pos->offset > offset)
            offset = pos->offset;

        if (pos->length > length)
            length = pos->length;
    }

    for (i = offset; i != 0; i = i >> 1, lz->offset_bits++);
    for (i = length; i != 0; i = i >> 1, lz->length_bits++);

    pair_bit_sz = lz->offset_bits + lz->length_bits;

    ks_list_for_each_entry(pos, &lz->pairs, pair_t, e)
    {
        bits += 1 + 8;
        if (pos->length != 0)
            bits += pair_bit_sz;
    }

    return (bits + 8 - 1) / 8;
}

unsigned char* lz77_header_save(lz77_t* lz)
{
    lz->nbyte = build_header(lz);
    lz->nbyte += 1;
    lz->all_bytes = (unsigned char*)calloc(1, lz->nbyte);

    lz->all_bytes[0] = ((lz->offset_bits & 0xF) << 4) + (lz->length_bits & 0xF);

    return &lz->all_bytes[1];
}

void lz77_header_load(lz77_t* lz)
{
    ks_unused(lz);
}