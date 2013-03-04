#ifndef __KS_TEXT_H__
#define __KS_TEXT_H__

#include <ks/defs.h>
#include <ks/node.h>
#include <ks/image.h>

typedef struct ks_text_t
{
    ks_extends_node();
    
    char*           file;
    char*           text;
    ks_image_t*     img;

} ks_text_t;

KS_API ks_text_t*   ks_text_new(const char* fontfile, ks_node_t* parent);
KS_API int          ks_text_set_content(ks_text_t* me, const char* content);


#endif
