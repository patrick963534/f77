#ifndef __KS_ZIP_H__
#define __KS_ZIP_H__

#include <ks/defs.h>



KS_API char* ks_zip_compress(const char* data, int sz, int* ret_sz);
KS_API char* ks_zip_uncompress(const char* data, int sz, int* ret_sz);

#endif