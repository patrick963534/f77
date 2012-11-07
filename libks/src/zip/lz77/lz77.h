#ifndef __KS_ZIP_LZ77_H__
#define __KS_ZIP_LZ77_H__

#include <ks/defs.h>

char* zip_lz77_compress(const char* data, int sz, int* ret_sz);
char* zip_lz77_uncompress(const char* data, int sz, int* ret_sz);

#endif