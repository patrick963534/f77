#ifndef __KS_PATH_H__
#define __KS_PATH_H__

#include <ks/defs.h>

KS_API char*   ks_path_basename(char* buf, int i, const char* file);
KS_API char*   ks_path_folder(char* buf, int sz, const char* file);
KS_API char*   ks_path_extention(char* buf, int sz, const char* file);
KS_API char*   ks_path_without_ext(char* buf, int sz, const char* file);
KS_API char*   ks_path_join(char* buf, int sz, const char* folder, const char* filename);

KS_API void    ks_path_enumerate_files(const char* path, void (*cb)(const char* file, const void* target));
KS_API void    ks_path_enumerate_folders(const char* path, void (*cb)(const char* folder, const void* target));

#endif
