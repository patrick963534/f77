#ifndef __KS_ZIP_HUFMAN_H__
#define __KS_ZIP_HUFMAN_H__

#include <ks/defs.h>

char* zip_hufman_compress(const char* data, int sz, int* ret_sz);
char* zip_hufman_uncompress(const char* data, int sz, int* ret_sz);

#endif