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

static void print_matrix(float* m1)
{
    int i;

    for (i = 0; i < 16; ++i)
    {
        if (i % 4 == 0)
            printf("\n");

        printf("%12f,", m1[i]);
    }

    fflush(stdout);
}

static int matrix_equals(float* m1, float* m2)
{
    int i;
    float epsilon = 0.000001f;

    for (i = 0; i < 16; ++i)
    {
        float diff = (m1[i] - m2[i]);
        if (diff > epsilon || diff < -epsilon)
            return 0;
    }

    return 1;
}

static void test_result(float* m1, float* m2, const char* name)
{
    if (!matrix_equals(m1, m2))
        ks_print_log("Error", "Test in function {%s} is fail.", name);
}

static void matrix_translate_test()
{
    float result[16] = {
        1,    0,    0,   0,
        0,    1,    0,   0,
        0,    0,    1,   0,
     0.5f, 0.3f, 0.2f,   1,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();
    sl_translate(0.5f, 0.3f, 0.2f);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void matrix_scale_test()
{
    float result[16] = {
        0.5f,    0,    0,   0,
           0, 0.3f,    0,   0,
           0,    0, 0.2f,   0,
           0,    0,    0,   1,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();
    sl_scale(0.5f, 0.3f, 0.2f);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void matrix_rotate_test()
{
    float result[16] = {
         0,  0, -1,  0,
         0,  1,  0,  0,
         1,  0,  0,  0,
        -5,  0,  0,  1,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();
    sl_rotate(90, 0, 1, 0);
    sl_translate(0, 0, -5);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void matrix_slu_perspective_test()
{
    float result[16] = {
        1.732051,    0.000000,    0.000000,    0.000000,
        0.000000,    1.732051,    0.000000,    0.000000,
        0.000000,    0.000000,   -1.000667,   -1.000000,
        0.000000,    0.000000,   -1.000333,    0.000000,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_model);
    sl_load_identity();
    slu_perspective(60, 800 / 480, 0.5, 1500);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

void ks_utest_sl_test()
{
    matrix_rotate_test();
    matrix_scale_test();
    matrix_translate_test();
    matrix_slu_perspective_test();
}
