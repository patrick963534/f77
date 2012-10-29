#ifndef __KS_LOG_H__
#define __KS_LOG_H__

#include <ks/defs.h>

KS_API void ks_print_log(const char *type, const char *format, ...);

#define ks_log(format, ...) ks_print_log("Info", format, __VA_ARGS__) 
#define ks_logE(format, ...) ks_print_log("Error", format, __VA_ARGS__) 

#endif
