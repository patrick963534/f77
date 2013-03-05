#ifndef __KS_TEXT_H__
#define __KS_TEXT_H__

#include <ks/defs.h>
#include <ks/node.h>
#include <ks/image.h>

typedef struct ks_text_t
{
    ks_extends_node();
    
    int             font_size;
    char*           font_file;
    char*           text;
    ks_image_t*     image;

} ks_text_t;

KS_API ks_text_t* ks_text_new(const char* font_file, int font_size, const char* content, ks_node_t* parent);
KS_API void       ks_text_set_content(ks_text_t* me, const char* content);
KS_API void       ks_text_destruct(ks_text_t* me);

#endif
