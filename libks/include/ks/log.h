#ifndef __KS_LOG_H__
#define __KS_LOG_H__

#include <ks/defs.h>

KS_API void ks_print_log(const char *type, const char *format, ...);

#define ks_log(format, args) ks_print_log("Info", format, args) 
#define ks_logE(format, args) ks_print_log("Error", format, args) 

#endif
