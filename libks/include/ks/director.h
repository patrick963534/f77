#ifndef __KS_DIRECTOR_H__
#define __KS_DIRECTOR_H__

#include <ks/defs.h>
#include <ks/container.h>
#include <ks/scene.h>

typedef struct ks_director_t
{
    ks_extends_container();
    
    char        title[256];
    int         width;
    int         height;

} ks_director_t;

KS_API void             ks_director_init(const char* title, int w, int h);
KS_API void             ks_director_run(ks_scene_t* scene);
KS_API ks_director_t*   ks_director_instance();

#endif
