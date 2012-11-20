#include "lz77.c.h"
#include <stdio.h>

static void build_pairs(lz77_t* lz)
{
    pair_t *pr;
    uchar  *win_pos;
    uchar  *cur, *end;
    int     cur_win_sz = 0;

    cur     = lz->bytes_uncompressed;
    end     = lz->bytes_uncompressed + lz->nbyte_uncompressed;
    win_pos = lz->bytes_uncompressed;

    lz->pairs = calloc(lz->nbyte_uncompressed, sizeof(lz->pairs[0]));
    lz->npair = 0;

    while (cur < end)
    {
        uchar* cp = cur;
        uchar* wp = win_pos;
        uchar* wp_end = win_pos + cur_win_sz;
        int length = 0;

        pr = &lz->pairs[lz->npair];
        pr->ch = *cp;
        lz->npair++;
        
        while (wp < wp_end)
        {
            while (*wp == *cp && wp < wp_end && cp < end)
            {
                length++;
                cp++;
                wp++;
            }

            if (length > pr->length)
            {
                pr->length = length;
                pr->offset = cp - wp;
                pr->ch = *cp;

                if (wp == wp_end)
                {
                    uchar* twp = cur - pr->offset;
                    uchar* tcp = cp;
                    while (*twp == *tcp && tcp < end)
                    {
                        twp++;
                        tcp++;

                        pr->length++;
                        pr->ch = *tcp;

                        if (twp == wp_end)
                            twp = cur - pr->offset;
                    }
                }
            }
            
            if (length == 0)
                wp++;

            cp = cur;
            length = 0;
        }

        cur        += pr->length + 1;
        cur_win_sz += pr->length + 1;

        if (cur_win_sz > MAX_WIN_SZ)
        {
            cur_win_sz = MAX_WIN_SZ;
            win_pos    = cur - MAX_WIN_SZ;
        }
    }

    lz->max_win_sz = cur_win_sz;
}

static void print_tree(lz77_t* lz)
{
    pair_t* pos;
    int i;

    for (i = 0; i < lz->npair; i++)
    {
        pos = &lz->pairs[i];
        printf("(%2d, %2d) %c \n", pos->offset, pos->length, pos->ch);
    }

    fflush(stdout);
}

static uchar* generate(lz77_t* lz, int* ret_sz)
{
    pair_t* pos;
    uchar*  bytes;
    int     offset_bits;
    int     length_bits;
    int     nbit;
    int     nbyte;
    uchar   b;
    int     i, np;

    bytes = (uchar*)lz77_header_save(lz);
    offset_bits = lz->offset_bits;
    length_bits = lz->length_bits;
    nbit  = 0;
    nbyte = 0;
    b     = 0;
    
    for (np = 0; np < lz->npair; np++)
    {
        uchar ch;
        pos = &lz->pairs[np];
        ch = pos->ch;

        if (pos->length != 0)
            b += 1 << (7 - nbit);
        
        nbit++;
        if (nbit == 8)
        {
            bytes[nbyte++] = b;
            nbit = 0;
            b = 0;
        }

        for (i = 0; i < 8; i++)
        {
            if (((ch >> (7 - i)) & 0x1))
                b += 1 << (7 - nbit);

            nbit++;
            if (nbit == 8)
            {
                bytes[nbyte++] = b;
                nbit = 0;
                b = 0;
            }
        }

        if (pos->length != 0)
        {
            int t_offset = pos->offset;
            int t_length = pos->length;
            for (i = 0; i < offset_bits; i++)
            {
                if (((t_offset >> (offset_bits - 1 - i)) & 0x1))
                    b += 1 << (7 - nbit);

                nbit++;
                if (nbit == 8)
                {
                    bytes[nbyte++] = b;
                    nbit = 0;
                    b = 0;
                }
            }

            for (i = 0; i < length_bits; i++)
            {
                if (((t_length >> (length_bits - 1 - i)) & 0x1))
                    b += 1 << (7 - nbit);

                nbit++;
                if (nbit == 8)
                {
                    bytes[nbyte++] = b;
                    nbit = 0;
                    b = 0;
                }
            }
        }
    }

    bytes[nbyte++] = b;
    *ret_sz = lz->nbyte_compressed;
    return lz->bytes_compressed;
}

char* zip_lz77_compress(uchar* data, int sz, int* ret_sz)
{
    lz77_t* lz;
    char* result;

    lz = calloc(1, sizeof(*lz));
    lz->bytes_uncompressed = (uchar*)data;
    lz->nbyte_uncompressed = sz;

    build_pairs(lz);
    print_tree(lz);
    generate(lz, ret_sz);
    result = (char*)lz->bytes_compressed;

    lz77_delete(lz);

    return result;
}
