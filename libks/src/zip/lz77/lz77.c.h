#ifndef __KS_ZIP_LZ77_C_H__
#define __KS_ZIP_LZ77_C_H__

#include <ks/defs.h>
#include <ks/list.h>
#include <ks/log.h>
#include <ks/helper.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define MAX_WIN_SZ  2048

typedef unsigned char   uchar;

typedef struct pair_t
{
    ks_list_t e;

    int             offset;
    int             length;
    unsigned char   ch;
} pair_t;

typedef struct lz77_t
{
    int         max_win_sz;

    ks_list_t   pairs;
    int         npair;

    int         offset_bits;
    int         length_bits;

    unsigned char*  bytes_uncompressed;
    int             nbyte_uncompressed;

    unsigned char*  bytes_compressed;
    int             nbyte_compressed;

} lz77_t;

unsigned char*  lz77_header_save(lz77_t* lz);
unsigned char*  lz77_header_load(lz77_t* lz);

#endif
