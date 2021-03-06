#include <ks/zip.h>
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>
#include "ks/helper.h"

static void testing(const char* data, int sz, ks_zip_type_e type)
{
    char* cdata;
    char* udata;
    int cret, uret, i;

    cdata = ks_zip_compress(data, sz, &cret, type);
    udata = ks_zip_uncompress(cdata, cret, &uret, type);

    if (uret != sz)
    {
        ks_log("%s, (%d, %d)", "not the same size.", uret, sz);
        goto fail;
    }

    for (i = 0; i < uret; i++)
    {
        if (data[i] != udata[i])
        {
            ks_log("%s : %d", "not the same content.", i);
            goto fail;
        }
    }

    free(udata);
    free(cdata);

    return;
fail:
    free(udata);
    free(cdata);
}

static void test_hufman_1()
{
    int i;
    int sz = 65536;
    char* data = malloc(sz);

    for (i = 0; i < sz; i++)
        data[i] = rand() % 256;

    testing(data, sz, ks_zip_type_hufman);

    free(data);
}

static void test_hufman_2()
{
    const char* data = "aabdakliaeaaaiesaasdfeawfasdfadfasdfadfaefeafagafdfaefsdfadfa";
    int sz = strlen(data) + 1;

    testing(data, sz, ks_zip_type_hufman);
}

static void test_hufman_3()
{
    const char* data = "good morning.";

    testing(data, strlen(data) + 1, ks_zip_type_hufman);
}

static void test_hufman_4()
{
    int i;
    int sz = 256;
    char* data = malloc(sz);

    for (i = 0; i < sz; i++)
        data[i] = (char)i;

    testing(data, sz, ks_zip_type_hufman);

    free(data);
}

static void test_lz77_1()
{
    const char* data = "good good study and day day up.";

    testing(data, strlen(data) + 1, ks_zip_type_lz77);
}

static void test_lz77_2()
{
    int i;
    int sz = 65536;
    char* data = malloc(sz);

    for (i = 0; i < sz; i++)
        data[i] = rand() % 26;

    testing(data, sz, ks_zip_type_lz77);

    free(data);
}

static void test_lz77_3()
{
    int     sz;
    char*   data;
    char    buf[256];

    ks_helper_path_join_relative_app(buf, sizeof(buf), "test_data_1.c");
    data = ks_helper_file_read_all(buf, &sz);

    testing(data, sz, ks_zip_type_lz77);

    free(data);
}

static void test_lz77_4()
{
    int     sz;
    char*   data;
    char    buf[256];

    ks_helper_path_join_relative_app(buf, sizeof(buf), "libks.exe");
    data = ks_helper_file_read_all(buf, &sz);

    testing(data, sz, ks_zip_type_lz77);

    free(data);
}

static void test_lz77_5()
{
    const char* data = "good good good good good good study.";

    testing(data, strlen(data) + 1, ks_zip_type_lz77);
}

static void test_deflate_draft_1()
{
    const char* data = "good morning.";

    testing(data, strlen(data) + 1, ks_zip_type_deflate);
}

static void test_deflate_draft_2()
{
    const char* data = "oooooooooooooooooooooooooooooo.";

    testing(data, strlen(data) + 1, ks_zip_type_deflate);
}

static void test_deflate_draft_3()
{
    const char* data = "good good good good good good study.";

    testing(data, strlen(data) + 1, ks_zip_type_deflate);
}

void ks_utest_zip_test()
{
    test_hufman_1();
    test_hufman_2();
    test_hufman_3();
    test_hufman_4();

    test_lz77_1();
    test_lz77_2();
    test_lz77_3();
    test_lz77_4();
    test_lz77_5();

    test_deflate_draft_1();
    test_deflate_draft_2();
    test_deflate_draft_3();
}
