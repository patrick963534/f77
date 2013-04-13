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

static void matrix_print(float* m1)
{
    int i;

    for (i = 0; i < 16; ++i)
    {
        if (i % 4 == 0)
            printf("\n");

        printf("%12ff,", m1[i]);
    }

    printf("\n");
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

static void matrix_load_identity_test()
{
    float result[16] = {
        1,    0,    0,   0,
        0,    1,    0,   0,
        0,    0,    1,   0,
        0,    0,    0,   1,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_modelview);
    sl_translate(0.5f, 0.3f, 0.2f);
    sl_load_identity();
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
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
    sl_matrix_mode(sl_matrix_mode_modelview);
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
    sl_matrix_mode(sl_matrix_mode_modelview);
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
    sl_matrix_mode(sl_matrix_mode_modelview);
    sl_rotate(90, 0, 1, 0);
    sl_translate(0, 0, -5);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void matrix_slu_perspective_test()
{
    float result[16] = {
        1.039230f,    0.000000f,    0.000000f,    0.000000f,
        0.000000f,    1.732051f,    0.000000f,    0.000000f,
        0.000000f,    0.000000f,   -1.000667f,   -1.000000f,
        0.000000f,    0.000000f,   -1.000333f,    0.000000f,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_modelview);
    slu_perspective(60, 800.f / 480.f, 0.5, 1500);
    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void matrix_push_pop_test()
{
    float result[16] = {
        0.5f,    0,    0,   0,
           0, 0.3f,    0,   0,
           0,    0, 0.2f,   0,
           0,    0,    0,   1,
    };

    float identity[16] = {
        1,    0,    0,   0,
        0,    1,    0,   0,
        0,    0,    1,   0,
        0,    0,    0,   1,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_modelview);
    sl_scale(0.5f, 0.3f, 0.2f);
    sl_push_matrix();
    sl_load_identity();

    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    test_result(identity, buf, __FUNCTION__);

    sl_pop_matrix();

    sl_get_floatv(sl_param_name_matrix_model_view, buf);    
    test_result(result, buf, __FUNCTION__);

    sl_close();
}

static void matrix_sl_frustum_test()
{
    float result[16] = {
        5.000000f,    0.000000f,    0.000000f,    0.000000f,
        0.000000f,    5.000000f,    0.000000f,    0.000000f,
        0.000000f,    0.000000f,   -2.000000f,   -1.000000f,
        0.000000f,    0.000000f,  -15.000000f,    0.000000f,
    };

    float buf[16];

    sl_init();
    sl_matrix_mode(sl_matrix_mode_project);
    sl_frustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
    sl_get_floatv(sl_param_name_matrix_projection, buf);    
    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void matrix_sl_mvp_test()
{
    float result[16] = {
        5.000000f,    0.000000f,    0.000000f,    0.000000f,
        0.000000f,    5.000000f,    0.000000f,    0.000000f,
        0.000000f,    0.000000f,   -2.000000f,   -1.000000f,
        0.000000f,    0.000000f,   -5.000000f,    5.000000f,
    };

    float buf[16];

    sl_init();

    sl_matrix_mode(sl_matrix_mode_project);
    sl_frustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
    sl_matrix_mode(sl_matrix_mode_modelview);
    sl_translate(.0f, .0f, -5.0f);

    sl_begin();
    slut_get_mvp(buf);
    sl_end();

    sl_close();

    test_result(result, buf, __FUNCTION__);
}

static void sl_render_test()
{
#define G   0xff00ff00
#define W   0xffffffff

    int tex_w = 8;
    int tex_h = 8;
    unsigned pixels[] = {
        W,W,W,W,W,W,W,W,
        W,W,W,G,G,W,W,W,
        W,W,G,W,W,G,W,W,
        W,W,W,W,W,G,W,W,
        W,W,W,W,G,W,W,W,
        W,W,W,G,W,W,W,W,
        W,W,G,G,G,G,W,W,
        W,W,W,W,W,W,W,W,
    };

#undef G
#undef W

    int width = 32;
    int height = 32;

    sl_init();
    sl_viewport(0, 0, width, height);

    sl_matrix_mode(sl_matrix_mode_project);
    sl_frustum(-1.0f, 1.0f, -1.0f, 1.0f, 5.0f, 15.0f);
    sl_matrix_mode(sl_matrix_mode_modelview);
    sl_translate(0.1f, 0, -5);

    sl_texture_load(tex_w, tex_h, pixels);
    sl_begin();
        sl_tex_coord(0.0, 0.0); sl_vertex( 0.0f, 0.5f, 0.f);
        sl_tex_coord(1.0, 0.0); sl_vertex( 0.5f, 0.5f, 0.f);
        sl_tex_coord(1.0, 1.0); sl_vertex( 0.5f, 0.0f, 0.f);

        sl_tex_coord(0.0, 0.0); sl_vertex( 0.0f,  0.5f, 0.f);
        sl_tex_coord(0.0, 1.0); sl_vertex( 0.0f,  0.0f, 0.f);
        sl_tex_coord(1.0, 1.0); sl_vertex( 0.5f,  0.0f, 0.f);
    sl_end();

    ks_helper_image565_save_ppm("sl_texture_mapping_test.ppm", sl_flush(), width, height);

    sl_close();
}

void ks_utest_sl_test()
{
    matrix_rotate_test();
    matrix_scale_test();
    matrix_translate_test();
    matrix_slu_perspective_test();
    matrix_load_identity_test();
    matrix_push_pop_test();
    matrix_sl_frustum_test();
    matrix_sl_mvp_test();
    sl_render_test();
}
