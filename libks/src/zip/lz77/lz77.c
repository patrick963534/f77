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

uchar* lz77_header_save(lz77_t* lz)
{
    lz->nbyte_compressed =  build_header(lz);
    lz->nbyte_compressed += 5;
    lz->bytes_compressed = (uchar*)calloc(1, lz->nbyte_compressed);

    lz->bytes_compressed[0] = ((lz->offset_bits & 0xF) << 4) + (lz->length_bits & 0xF);
    ks_helper_int_to_bytes(&lz->bytes_compressed[1], lz->nbyte_uncompressed);

    return &lz->bytes_compressed[5];
}

uchar* lz77_header_load(lz77_t* lz)
{
    uchar* data = lz->bytes_compressed;

    lz->offset_bits = (data[0] >> 4) & 0xF;
    lz->length_bits = (data[0] & 0xF);

    ks_helper_bytes_to_int(&data[1], &lz->nbyte_uncompressed);
    lz->bytes_uncompressed = calloc(1, lz->nbyte_uncompressed);

    return &data[5];
}