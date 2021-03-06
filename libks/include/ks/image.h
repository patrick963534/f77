#ifndef __KS_IMAGE_H__
#define __KS_IMAGE_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/object.h>

#define ks_extends_image()  \
    ks_extends_object();    \
    char* pixels;           \
    const char* file;       \
    int width;              \
    int height

typedef struct ks_image_t
{
    ks_extends_image();
} ks_image_t;

KS_API ks_image_t*  ks_image_load(const char* file, ks_object_t* container);
KS_API ks_image_t*  ks_image_from(const char* pixels, int width, int height, ks_object_t* container);
KS_API void         ks_image_save(ks_image_t* me, const char* file);
KS_API void         ks_image_destruct(ks_image_t* me);

#endif
