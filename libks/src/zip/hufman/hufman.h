#ifndef __KS_ZIP_HUFMAN_H__
#define __KS_ZIP_HUFMAN_H__

#include <ks/defs.h>

char* ks_zip_hufman_compress(const char* data);
char* ks_zip_hufman_uncompress(const char* data);

#endif