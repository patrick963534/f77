#ifndef __KS_LOG_H__
#define __KS_LOG_H__

#include <ks/defs.h>

KS_API void ks_print_log(const char *type, const char *format, ...);

#define ks_log(...) ks_print_log("Info", __VA_ARGS__) 
#define ks_logE(...) ks_print_log("Error", __VA_ARGS__) 

#endif
