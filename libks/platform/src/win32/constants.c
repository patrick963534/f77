#include <ks/constants.h>

#define KS_KEYS_VALUE_ESCAPE    (10)
#define KS_KEYS_VALUE_UP        (10)
#define KS_KEYS_VALUE_DOWN      (10)
#define KS_KEYS_VALUE_LEFT      (10)
#define KS_KEYS_VALUE_RIGHT     (10)

KS_API const ks_constants_t ks = {
    { // KEYS
        #define KS_KEYS_CONSTANT(Name, Field, Value)    KS_KEYS_VALUE_ ## Name,
        #include <ks/constants.keys.h>
        #undef KS_KEYS_CONSTANT
    }
};
