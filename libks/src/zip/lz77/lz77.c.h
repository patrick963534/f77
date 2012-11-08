#ifndef __KS_ZIP_LZ77_C_H__
#define __KS_ZIP_LZ77_C_H__

#include <ks/defs.h>
#include <ks/list.h>
#include <ks/log.h>
#include <stdlib.h>

typedef struct pair_t
{
    ks_list_t e;

    int     offset;
    int     length;
    char    ch;
} pair_t;

typedef struct lz77_t
{
    int         max_win_sz;
    int         cur_win_sz;

    ks_list_t   pairs;
    int         npair;

} lz77_t;

#endif