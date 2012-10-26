#ifndef __KS_ZIP_H__
#define __KS_ZIP_H__

#include <ks/defs.h>

KS_API char* ks_zip_compress(const char* data);
KS_API char* ks_zip_uncompress(const char* data);

#endif