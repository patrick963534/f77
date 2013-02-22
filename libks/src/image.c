#include <ks/image.h>
#include <ks/container.h>
#include <ks/libc.h>
#include <ks/list.h>

#include <stdlib.h>
#include <string.h>

#include "img/img_loader.h"

typedef struct cached_image_t
{
    char* pixels;
    char* file;
    int   w;
    int   h;

    ks_list_t item;
    int       ref_count;
} cached_image_t;

static ks_list_t head = {&head, &head};

static cached_image_t* cached_image_search(const char* file)
{
    cached_image_t* pos;

    if (!file)
        return 0;

    ks_list_for_each_entry(pos, &head, cached_image_t, item)
    {
        if (strcmp(pos->file, file) == 0)
            return pos;
    }

    return 0;
}

static cached_image_t* cached_image_load(const char* file)
{
    cached_image_t* img = cached_image_search(file);

    if (!img)
    {
        so_img_loader_data_t img_data;
        so_img_loader_load(file, &img_data);

        img             = (cached_image_t*)calloc(1, sizeof(*img));
        img->pixels     = img_data.pixels;
        img->w          = img_data.width;
        img->h          = img_data.height;
        img->file       = ks_strdup(file);
        img->ref_count  = 1;

        ks_list_init(&img->item);
        ks_list_add_tail(&head, &img->item);
    }

    return img;
}

static void cached_image_remove(const char* file)
{
    cached_image_t* img = cached_image_search(file);

    if (img && --img->ref_count <= 0)
    {
        ks_list_remove(&img->item);
        free(img->pixels);
        free(img->file);
        free(img);
    }
}

KS_API void ks_image_destruct(ks_image_t* me)
{
    cached_image_remove(me->file);

    ks_object_destruct((ks_object_t*)me);
}

KS_API void ks_image_save(ks_image_t* me, const char* file)
{
    ks_unused(me);
    ks_unused(file);
}

KS_API ks_image_t* ks_image_load(const char* file, ks_container_t* container)
{
    ks_image_t* me;
    cached_image_t* img;

    img = cached_image_load(file);

    me           = (ks_image_t*)ks_object_new(sizeof(*me));
    me->tname    = "image";
    me->destruct = (ks_destruct_f)ks_image_destruct;
    me->file     = img->file;
    me->pixels   = img->pixels;
    me->width    = img->w;
    me->height   = img->h;

    if (container)
        ks_container_add(container, (ks_object_t*)me);

    return me;
}
