#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>
#include <ks/system.h>
#include <stdlib.h>
#include <string.h>
#include <ks/director.h>
#include <ks/scene.h>
#include "utest/utest.h"

int main()
{
    ks_utest_start();

    ks_director_init("Hello World", 320, 240);
    ks_director_run(ks_scene_new(sizeof(ks_scene_t)));
    return 0;
}
