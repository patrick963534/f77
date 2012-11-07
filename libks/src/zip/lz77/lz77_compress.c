#include "lz77.c.h"

static void build_pairs(lz77_t* lz, const char* data, int sz)
{
    pair_t* pr;

    const char *win_pos;
    const char *cur, *end;

    int win_sz = 1;
    int max_win_sz = 1024;

    cur = data;
    end = data + sz;

    pr = calloc(1, sizeof(*pr));
    pr->ch = *cur;
    ks_list_init(&pr->e);
    ks_list_add_tail(&lz->pairs, &pr->e);

    cur++;
    win_pos = data;

    while (cur < end)
    {
        const char* cp = cur;
        const char* wp = win_pos;
        int length = 0;

        pr = calloc(1, sizeof(*pr));
        pr->ch = *cp;
        ks_list_init(&pr->e);
        ks_list_add_tail(&lz->pairs, &pr->e);

        if (win_sz < max_win_sz)
            win_sz++;
        
        while (wp < win_pos + win_sz)
        {
            if (*wp == *cp)
            {
                length++;
                cp++;
            }
            else if (length != 0)
            {
                if (pr->length < length)
                {
                    pr->length = length;
                    pr->offset = wp - win_pos;
                    pr->ch = *cp;
                }
                
                length = 0;
                cp = cur;
            }

            wp++;            
        }
    }
}

char* zip_lz77_compress(const char* data, int sz, int* ret_sz)
{
    lz77_t* lz;

    lz = calloc(1, sizeof(*lz));
    ks_list_init(&lz->pairs);

    build_pairs(lz, data, sz);

    *ret_sz = 0;

    return 0;
}

char* zip_lz77_uncompress(const char* data, int sz, int* ret_sz)
{
    ks_unused(data);
    ks_unused(sz);
    ks_unused(ret_sz);
    return 0;
}
