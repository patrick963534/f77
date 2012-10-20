#include <ks/image.h>
#include <ks/container.h>
#include <ks/libc.h>

static unsigned char* load_bvg_image(const char* file, int* width, int* height)
{
    ks_unused(file);
    ks_unused(width);
    ks_unused(height);

    return 0;
}

static void save_bvg_image(const char* dst, unsigned char* pixels, int w, int h)
{
    ks_unused(dst);
    ks_unused(pixels);
    ks_unused(w);
    ks_unused(h);
}

KS_API void ks_image_destruct(ks_image_t* me)
{
    ks_object_destruct((ks_object_t*)me);
}

KS_API void ks_image_save(ks_image_t* me, const char* file)
{
    save_bvg_image(file, me->pixels, me->width, me->height);
}

KS_API ks_image_t* ks_image_load(const char* file, ks_container_t* container)
{
    ks_image_t* me;

    me           = (ks_image_t*)ks_object_new(sizeof(*me));
    me->file     = ks_strdup(file);
    me->tname    = "image";
    me->destruct = (ks_destruct_f)ks_image_destruct;
    me->pixels   = load_bvg_image(file, &me->width, &me->height);

    if (container)
        ks_container_add(container, (ks_object_t*)me);

    return me;
}
