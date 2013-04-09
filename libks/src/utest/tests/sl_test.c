#include "path_test.h"
#include <ks/image.h>
#include <ks/log.h>
#include <ks/helper.h>
#include <string.h>
#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>
#include <ks/system.h>
#include <stdlib.h>
#include <string.h>
#include <ks/director.h>
#include <ks/scene.h>
#include <ks/actor.h>
#include <ks/helper.h>
#include <ks/event.h>
#include <ks/text.h>
#include <ks/libc.h>
#include <ks/constants.h>
#include <sl/sl.h>

static void matrix_test()
{
    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();
    sl_rotate(90, 0, 1, 0);
    sl_translate(0, 0, -5);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();
}

void ks_utest_sl_test()
{
    matrix_test();
}
