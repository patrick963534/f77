#ifndef __KS_HELPER_H__
#define __KS_HELPER_H__

#include <ks/defs.h>

KS_API int      ks_helper_file_get_length(const char* file);
KS_API void     ks_helper_file_read_all(char* buf, int sz, const char* file);

KS_API void     ks_helper_int_to_bytes(char* buf, int val);
KS_API void     ks_helper_bytes_to_int(const char* buf, int* val);

#endif