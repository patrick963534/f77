#ifndef __KS_HASH_H__
#define __KS_HASH_H__

#include <ks/defs.h>

typedef struct ks_hash_t ks_hash_t;

KS_API ks_hash_t* ks_hash_new(int cap);
KS_API void       ks_hash_add(ks_hash_t* me, const char* key, void* val);

#endif