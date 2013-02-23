#ifndef __KS_ENVIRONMENT_C_H__
#define __KS_ENVIRONMENT_C_H__

#include <ks/environment.h>
#include <ks/object.h>
#include <ks/object.h>

typedef struct ks_environment_t
{
    ks_extends_object();

    char*   app_path;
} ks_environment_t;

void ks_environment_init(int argc, char** argv, ks_object_t* container);

#endif
