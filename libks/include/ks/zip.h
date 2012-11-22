#ifndef __KS_ZIP_H__
#define __KS_ZIP_H__

#include <ks/defs.h>

typedef enum ks_zip_type_e
{
    ks_zip_type_hufman,
    ks_zip_type_lz77,
    ks_zip_type_deflate_draft,
} ks_zip_type_e;

KS_API char* ks_zip_compress(const char* data, int sz, int* ret_sz, ks_zip_type_e type);
KS_API char* ks_zip_uncompress(const char* data, int sz, int* ret_sz, ks_zip_type_e type);

#endif