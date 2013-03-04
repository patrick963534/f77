#include <ks/text.h>
#include <ks/log.h>
#include <ks/libc.h>
#include <stdlib.h>
#include "font/font_loader.h"

KS_API void ks_text_destruct(ks_node_t* me)
{
    ks_node_destruct(me);
}

KS_API ks_text_t* ks_text_new(const char* font_file, int font_size, ks_node_t* parent)
{
    ks_text_t* me;

    me              = (ks_text_t*)ks_node_new(sizeof(*me), parent);
    me->font_file   = ks_strdup(font_file);
    me->font_size   = font_size;
    me->destruct    = (ks_destruct_f)ks_text_destruct;
    me->tname       = "ks_text";

    return me;
}

KS_API void ks_text_set_content(ks_text_t* me, const char* content)
{
    so_font_loader_data_t data;
    so_font_loader_load(me->font_file, me->font_size, content, &data);
}
