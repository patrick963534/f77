#ifndef __KS_HELPER_H__
#define __KS_HELPER_H__

#include <ks/defs.h>

KS_API void     ks_helper_int_to_bytes(char* buf, int val);
KS_API void     ks_helper_bytes_to_int(const char* buf, int* val);

#endif