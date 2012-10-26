#include <ks/zip.h>
#include "hufman/hufman.h"

KS_API char* ks_zip_compress(const char* data)
{
    return ks_zip_hufman_compress(data);
}

KS_API char* ks_zip_uncompress(const char* data)
{
    return ks_zip_hufman_uncompress(data);
}