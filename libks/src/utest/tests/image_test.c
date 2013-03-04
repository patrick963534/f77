#include "path_test.h"
#include <ks/image.h>
#include <ks/log.h>
#include <ks/helper.h>
#include <string.h>

static void image_load_test()
{
    ks_image_t* img;

    img = ks_image_load("aa.png", NULL);

    ks_object_delete(img);
}

void ks_utest_image_test()
{
    image_load_test();
}
