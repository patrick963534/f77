#ifndef __KS_FONT_H__
#define __KS_FONT_H__

#include <ks/defs.h>
#include <ks/object.h>

typedef struct ks_font_t
{
    ks_extends_object();
    
    char*           file;

} ks_font_t;

KS_API ks_font_t* ks_font_new(const char* fontfile, ks_object_t* container);
KS_API void       ks_font_destruct(ks_font_t* me);

#endif
