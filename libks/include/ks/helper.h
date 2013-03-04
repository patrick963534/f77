#ifndef __KS_HELPER_H__
#define __KS_HELPER_H__

#include <ks/defs.h>

KS_API char*    ks_helper_path_join_relative_app(char* buf, int sz, const char* filename);

KS_API int      ks_helper_file_get_length(const char* file);
KS_API char*    ks_helper_file_read_all(const char* file, int* ret_sz);
KS_API int      ks_helper_file_exist(const char* file);

KS_API void     ks_helper_int_to_bytes(char* buf, int val);
KS_API void     ks_helper_bytes_to_int(const char* buf, int* val);

KS_API void     ks_helper_image_save_ppm(const char* file, const char* pixels, int width, int height);

#endif