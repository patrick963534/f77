#include <ks/zip.h>
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>
#include "test_data.h"

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
        return;
    }

    for (i = 0; i < uret; i++)
    {
        if (data[i] != udata[i])
        {
            ks_log("%s : %d", "not the same content.", i);
            return;
        }
    }

    return;
}

static void test_hufman_1()
{
    int i;
    int sz = 65536;
    char* data = malloc(sz);

    for (i = 0; i < sz; i++)
        data[i] = rand() % 26;

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
    int sz;
    const char* data = ks_utest_test_data_get_code_file(&sz);

    testing(data, sz, ks_zip_type_lz77);
}

void ks_utest_zip_test()
{
    test_hufman_3();
    test_hufman_1();
    test_hufman_2();

    test_lz77_1();
    test_lz77_2();
    test_lz77_3();
}
