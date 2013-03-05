#include <ks/text.h>
#include <ks/log.h>
#include <ks/libc.h>
#include <ks/graphics.h>
#include <stdlib.h>
#include "font/font_loader.h"

static void text_draw(ks_text_t* me)
{
    if (!me->image)
        return;

    ks_graphics_draw(me->image, 0, 0, 0, 0, me->image->width, me->image->height);
}

KS_API void ks_text_destruct(ks_text_t* me)
{
    free(me->text);
    ks_node_destruct((ks_node_t*)me);
}

KS_API ks_text_t* ks_text_new(const char* font_file, int font_size, const char* content, ks_node_t* parent)
{
    ks_text_t* me;

    me              = (ks_text_t*)ks_node_new(sizeof(*me), parent);
    me->font_file   = ks_strdup(font_file);
    me->font_size   = font_size;
    me->destruct    = (ks_destruct_f)ks_text_destruct;
    me->draw        = (ks_draw_f)text_draw;
    me->tname       = "ks_text";

    ks_text_set_content(me, content);

    return me;
}

KS_API void ks_text_set_content(ks_text_t* me, const char* content)
{
    so_font_loader_data_t data;
    
    if (content == NULL)
        return;

    so_font_loader_load(me->font_file, me->font_size, content, &data);
    
    ks_object_delete(me->image);
    free(me->text);

    me->text  = ks_strdup(content);
    me->image = ks_image_from(data.pixels, data.width, data.height, (ks_object_t*)me);
}
