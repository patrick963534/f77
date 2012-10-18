#include <ks/constants.h>
#include <windows.h>

#define KS_KEYS_VALUE_ESCAPE    (VK_ESCAPE)
#define KS_KEYS_VALUE_UP        (VK_UP)
#define KS_KEYS_VALUE_DOWN      (VK_DOWN)
#define KS_KEYS_VALUE_LEFT      (VK_LEFT)
#define KS_KEYS_VALUE_RIGHT     (VK_RIGHT)

#define KS_TYPES_KEY_DOWN       (WM_KEYDOWN)
#define KS_TYPES_KEY_UP         (WM_KEYUP)
#define KS_TYPES_KEY_CHAR       (WM_CHAR)

KS_API const ks_constants_t ks = {
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
