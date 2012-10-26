#ifndef __KS_ZIP_HUFMAN_H__
#define __KS_ZIP_HUFMAN_H__

#include <ks/defs.h>

char* ks_zip_hufman_compress(const char* data, int sz, int* ret_sz);
char* ks_zip_hufman_uncompress(const char* data, int sz, int* ret_sz);

#endif