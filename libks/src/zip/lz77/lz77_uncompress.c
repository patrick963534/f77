#include "lz77.c.h"


char* zip_lz77_uncompress(const char* data, int sz, int* ret_sz)
{
    lz77_t* lz;
    unsigned char* content;

    lz = calloc(1, sizeof(*lz));
    ks_list_init(&lz->pairs);

    content = lz77_header_load(lz, data, sz);

    ks_unused(data);
    ks_unused(sz);
    ks_unused(ret_sz);
    return 0;
}
