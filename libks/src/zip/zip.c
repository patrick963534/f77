#include <ks/zip.h>
#include "hufman/hufman.h"

KS_API char* ks_zip_compress(const char* data, int sz, int* ret_sz)
{
    return ks_zip_hufman_compress(data, sz, ret_sz);
}

KS_API char* ks_zip_uncompress(const char* data, int sz, int* ret_sz)
{
    return ks_zip_hufman_uncompress(data, sz, ret_sz);
}