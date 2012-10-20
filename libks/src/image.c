#include <ks/image.h>
#include <ks/container.h>
#include <ks/libc.h>

KS_API void ks_image_destruct(ks_image_t* me)
{
    ks_object_destruct((ks_object_t*)me);
}

KS_API ks_image_t*  ks_image_load(const char* file, ks_container_t* container)
{
    ks_image_t* me;

    me           = (ks_image_t*)ks_object_new(sizeof(*me));
    me->file     = ks_strdup(file);
    me->tname    = "image";
    me->destruct = (ks_destruct_f)ks_image_destruct;

    if (container)
        ks_container_add(container, (ks_object_t*)me);

    return me;
}
