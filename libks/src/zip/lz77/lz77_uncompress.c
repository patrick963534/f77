#include "lz77.c.h"

static void generate(lz77_t* lz)
{
    uchar* bytes_uncompress;

    uchar* bytes;
    int    offset_bits;
    int    length_bits;
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

            nbit++;
            if (nbit == 8)
            {
                nbit = 0;
                nbyte++;
                b = bytes[nbyte];
            }
        }

        *bytes_uncompress++ = (uchar)to;

        if (is_pair)
        {
            int t_offset = 0;
            int t_length = 0;

            for (i = 0; i < offset_bits; i++)
            {
                if ((b >> (7 - nbit)) & 0x1)
                    t_offset += 1 << (offset_bits - 1 - i);

                nbit++;
                if (nbit == 8)
                {
                    nbit = 0;
                    nbyte++;
                    b = bytes[nbyte];
                }
            }

            for (i = 0; i < length_bits; i++)
            {
                if ((b >> (7 - nbit)) & 0x1)
                    t_length += 1 << (length_bits - 1 - i);

                nbit++;
                if (nbit == 8)
                {
                    nbit = 0;
                    nbyte++;
                    b = bytes[nbyte];
                }
            }

            for (i = 0; i < t_length; i++)
            {
                bytes_uncompress[i] = *(bytes_uncompress - t_offset + i);
            }

            bytes_uncompress += t_length;
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

    *ret_sz = lz->nbyte_uncompressed;

    return (char*)lz->bytes_uncompressed;
}
