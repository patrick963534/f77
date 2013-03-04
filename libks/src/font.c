#include <ks/font.h>
#include <ks/log.h>
#include <ks/libc.h>
#include <stdlib.h>

typedef struct cached_font_t
{
    char* file;

    ks_list_t item;
    int       ref_count;

} cached_font_t;

static ks_list_t head = {&head, &head};

static cached_font_t* cached_font_load(const char* file)
{
    cached_font_t* font = NULL;//cached_image_search(file);

    if (!font)
    {
//         so_img_loader_data_t img_data;
//         so_img_loader_load(file, &img_data);

        font             = (cached_font_t*)calloc(1, sizeof(*font));
        font->file       = ks_strdup(file);
        font->ref_count  = 1;

        ks_list_init(&font->item);
        ks_list_add_tail(&head, &font->item);
    }

    return font;
}

static void cached_image_remove(const char* file)
{
    cached_font_t* img = 0; //cached_image_search(file);
    ks_unused(file);
    if (img && --img->ref_count <= 0)
    {
        ks_log("free image: %s", img->file);
        ks_list_remove(&img->item);
        free(img->file);
        free(img);
    }
}


KS_API void ks_font_destruct(ks_font_t* me)
{
    cached_image_remove(me->file);
    ks_object_destruct((ks_object_t*)me);
}

KS_API ks_font_t* ks_font_new(const char* font_file, ks_object_t* container)
{
    ks_font_t* me;
    cached_font_t* img;

    img = cached_font_load(font_file);

    me           = (ks_font_t*)ks_object_new(sizeof(*me));
    me->tname    = "ks_font";
    me->destruct = (ks_destruct_f)ks_font_destruct;
    me->file     = img->file;

    if (container)
        ks_object_add(container, (ks_object_t*)me);

    return me;
}
