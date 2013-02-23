#include "environment.c.h"
#include <ks/libc.h>
#include <stdlib.h>

static ks_environment_t* env = (void*)0;

static void destruct(ks_object_t* me_)
{
    ks_cast(ks_environment_t);

    free(me->app_path);
    env = (void*)0;

    ks_object_destruct((ks_object_t*)me);
}

void ks_environment_init(int argc, char** argv, ks_object_t* container)
{
    ks_environment_t* me;

    me           = (ks_environment_t*)ks_object_new(sizeof(*me));
    me->destruct = (ks_destruct_f)destruct;
    me->app_path = ks_strdup(argv[0]);

    if (container)
        ks_object_add(container, (ks_object_t*)me);

    ks_unused(argc);
    ks_unused(argv);

    env = me;
}

KS_API const char* ks_environment_app_filepath()
{
    return env->app_path;
}

