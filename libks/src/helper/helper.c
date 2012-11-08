#include <ks/helper.h>
#include <stdio.h>

KS_API void ks_helper_int_to_bytes(char* buf, int val)
{
    buf[0] = (char)((val >> 24));
    buf[1] = (char)((val >> 16) & 0xFF);
    buf[2] = (char)((val >>  8) & 0xFF);
    buf[3] = (char)((val      ) & 0xFF);
}

KS_API void ks_helper_bytes_to_int(const char* buf, int* val)
{
    const unsigned char* b = (const unsigned char*)buf;

    *val = (int)((b[0] << 24) + (b[1] << 16) + (b[2] << 8) + b[3]);
}

KS_API int ks_helper_file_get_length(const char* file)
{
    FILE*   fp;
    int    len;

    fp = fopen(file, "rb");
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fclose(fp);

    return len;
}

KS_API int ks_helper_file_read_all(char* buf, int sz, const char* file)
{
    FILE*   fp;
    int     len;

    fp = fopen(file, "rb");
    len = fread(buf, sz, 1, fp);
    fclose(fp);

    return len;
}