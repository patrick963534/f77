#ifndef __KS_UTF8_H__
#define __KS_UTF8_H__

#include <ks/defs.h>

KS_API int          ks_u8_to_ucs(unsigned *dest, int sz, char *src, int srcsz);
KS_API int          ks_u8_from_ucs(char *dest, int sz, unsigned *src, int srcsz);
KS_API int          ks_u8_encode(char *dest, unsigned ch);
KS_API unsigned     ks_u8_next_ucs(char *s, int *i);
KS_API char*        ks_u8_chr(char *s, unsigned ch, int *charn);
KS_API int          ks_u8_lenth(char *s);

#endif