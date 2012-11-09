#include "lz77.c.h"

static void generate(lz77_t* lz)
{
    uchar* bytes_uncompress;

    uchar* bytes;
    int    offset_bits;
    int    length_bits;
    int    bit;
    int    nbit;
    int    nbyte;
    int    i;
    uchar  b;

    bytes = lz77_header_load(lz);

    bytes_uncompress = lz->bytes_uncompressed;
    offset_bits = lz->offset_bits;
    length_bits = lz->length_bits;

    nbit  = 0;
    nbyte = 0;
    b     = bytes[nbyte];  

    while (nbyte < lz->nbyte_compressed)
    {
        int is_pair = ((b >> (7 - nbit)) & 0x1);
        int to;

        nbit++;
        if (nbit == 8)
        {
            nbit = 0;
            nbyte++;
            b = bytes[nbyte];
        }

        to = 0;
        for (i = 0; i < 8; i++)
        {
            if ((b >> (7 - nbit)) & 0x1)
                to += 1 << (7 - i);


        }

        if (is_pair)
        {
            
        }
        
    }
}

char* zip_lz77_uncompress(const char* data, int sz, int* ret_sz)
{
    lz77_t* lz;

    lz = calloc(1, sizeof(*lz));
    ks_list_init(&lz->pairs);

    lz->bytes_compressed = (uchar*)data;
    lz->nbyte_compressed = sz;

    generate(lz);

    return (char*)lz->bytes_uncompressed;
}
