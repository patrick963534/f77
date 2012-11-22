#include <ks/zip.h>
#include "hufman/hufman.h"
#include "lz77/lz77.h"
#include "deflate_draft/deflate_draft.h"

KS_API char* ks_zip_compress(const char* data, int sz, int* ret_sz, ks_zip_type_e type)
{
    if (type == ks_zip_type_hufman)
        return zip_hufman_compress(data, sz, ret_sz);
    else if (type == ks_zip_type_lz77)
        return zip_lz77_compress(data, sz, ret_sz);
    else if (type == ks_zip_type_deflate_draft)
        return zip_deflate_draft_compress(data, sz, ret_sz);

    return 0;
}

KS_API char* ks_zip_uncompress(const char* data, int sz, int* ret_sz, ks_zip_type_e type)
{
    if (type == ks_zip_type_hufman)
        return zip_hufman_uncompress(data, sz, ret_sz);
    else if (type == ks_zip_type_lz77)
        return zip_lz77_uncompress(data, sz, ret_sz);
    else if (type == ks_zip_type_deflate_draft)
        return zip_deflate_draft_uncompress(data, sz, ret_sz);

    return 0;
}