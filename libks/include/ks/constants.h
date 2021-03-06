#ifndef __KS_CONSTANTS_H__
#define __KS_CONSTANTS_H__

#include <ks/defs.h>

typedef struct ks_constants_key_t
{
#define KS_KEYS_CONSTANT(Name, Field, Value)    int Field;
#include <ks/constants.keys.h>
#undef KS_KEYS_CONSTANT
} ks_constants_key_t;

typedef struct ks_constants_type_t
{
#define KS_TYPE_CONSTANT(Name)    int Name;
#include <ks/constants.types.h>
#undef KS_TYPE_CONSTANT
} ks_constants_type_t;

typedef struct ks_constants_t
{
    ks_constants_type_t types;
    ks_constants_key_t  keys;
} ks_constants_t;

KS_API const ks_constants_t ks;

#endif
