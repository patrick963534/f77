#ifndef __KS_DIRECTOR_H__
#define __KS_DIRECTOR_H__

#include <ks/defs.h>
#include <ks/object.h>
#include <ks/scene.h>

typedef struct ks_director_t
{
    ks_extends_object();

    char        title[256];
    int         width;
    int         height;

    ks_scene_t* scene;

} ks_director_t;

KS_API void             ks_director_init(const char* title, int w, int h, int argc, char** argv);
KS_API void             ks_director_run(ks_scene_t* scene);
KS_API ks_director_t*   ks_director_instance();

#endif
