#include <ks/path.h>
#include <windows.h>

KS_API void ks_path_enumerate_files(const char* path, void (*cb)(const char* file, const void* target))
{
    ks_unused(path);
    ks_unused(cb);
}

KS_API void ks_path_enumerate_folders(const char* path, void (*cb)(const char* folder, const void* target))
{
    ks_unused(path);
    ks_unused(cb);
}