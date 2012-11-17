#include "lz77.c.h"
#include <stdio.h>

static void build_pairs(lz77_t* lz)
{
    pair_t *pr;
    uchar  *win_pos;
    uchar  *cur, *end;

    uchar* data = lz->bytes_uncompressed;
    int    sz   = lz->nbyte_uncompressed;

    int cur_win_sz = 1;

    cur = data;
    end = data + sz;

    pr = calloc(1, sizeof(*pr));
    pr->ch = *cur;
    ks_list_init(&pr->e);
    ks_list_add_tail(&lz->pairs, &pr->e);
    lz->npair++;

    cur++;
    win_pos = data;

    while (cur < end)
    {
        uchar* cp = cur;
        uchar* wp = win_pos;
        int length = 0;

        pr = calloc(1, sizeof(*pr));
        pr->ch = *cp;
        ks_list_init(&pr->e);
        ks_list_add_tail(&lz->pairs, &pr->e);
        lz->npair++;
        
        while (wp < win_pos + cur_win_sz)
        {
            if (*wp == *cp)
            {
                length++;
                cp++;
            }
            else if (length != 0)
            {
                if (pr->length <= length)
                {
                    pr->length = length;
                    pr->offset = cp - wp;
                    pr->ch = *cp;
                }
                
                length = 0;
                cp = cur;
            }

            wp++;
        }

        if (length != 0)
        {
            pr->length = length;
            pr->offset = cp - wp;
            pr->ch = *cp;
        }

        cur += pr->length + 1;

        cur_win_sz += pr->length + 1;
        if (cur_win_sz > MAX_WIN_SZ)
        {
            cur_win_sz = MAX_WIN_SZ;
            win_pos = cur - MAX_WIN_SZ;
        }
    }

    lz->max_win_sz = cur_win_sz;
}

static void print_tree(lz77_t* lz)
{
    pair_t* pos;

    ks_list_for_each_entry(pos, &lz->pairs, pair_t, e)
    {
        //printf("(%2d, %2d) %c \n", pos->offset, pos->length, pos->ch);
    }

    //fflush(stdout);
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
    int     i;

    bytes = (uchar*)lz77_header_save(lz);
    offset_bits = lz->offset_bits;
    length_bits = lz->length_bits;
    nbit  = 0;
    nbyte = 0;
    b     = 0;
    
    ks_list_for_each_entry(pos, &lz->pairs, pair_t, e)
    {
        uchar ch = pos->ch;

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
    ks_list_init(&lz->pairs);

    lz->bytes_uncompressed = (uchar*)data;
    lz->nbyte_uncompressed = sz;

    build_pairs(lz);
    print_tree(lz);
    generate(lz, ret_sz);
    result = (char*)lz->bytes_compressed;

    lz77_delete(lz);

    return result;
}
