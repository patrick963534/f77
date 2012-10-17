#include <ks/constants.h>

#define KS_KEYS_VALUE_ESCAPE    (10)
#define KS_KEYS_VALUE_UP        (10)
#define KS_KEYS_VALUE_DOWN      (10)
#define KS_KEYS_VALUE_LEFT      (10)
#define KS_KEYS_VALUE_RIGHT     (10)

#define KS_TYPES_KEY_DOWN       (10)
#define KS_TYPES_KEY_UP         (10)

const ks_constants_t ks = {
    { // TYPES
        #define KS_TYPE_CONSTANT(Name) KS_TYPES_ ## Name,
        #include <ks/constants.types.h>
        #undef KS_TYPE_CONSTANT
    },
    { // KEYS
        #define KS_KEYS_CONSTANT(Name, Field, Value) KS_KEYS_VALUE_ ## Name,
        #include <ks/constants.keys.h>
        #undef KS_KEYS_CONSTANT
    }
};
