#ifndef __KS_ZIP_DEFLATE_H__
#define __KS_ZIP_DEFLATE_H__

char* zip_deflate_compress(const char* data, int sz, int* ret_sz);
char* zip_deflate_uncompress(const char* data, int sz, int* ret_sz);

#endif