#include <ks/zip.h>
#include <ks/log.h>
#include <stdlib.h>
#include <string.h>

static void test_1()
{
    char* data;
    char* cdata;
    char* udata;
    int cret, uret, sz, i;

    sz = 65536;
    data = malloc(sz);

    srand(0);

    for (i = 0; i < sz; i++)
        data[i] = rand() % 26;

    cdata = ks_zip_compress(data, sz, &cret);
    udata = ks_zip_uncompress(cdata, cret, &uret);

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
}

static void test_2()
{
    const char* data = "aabdakliaeaaaiesaasdfeawfasdfadfasdfadfaefeafagafdfaefsdfadfa";
    int sz;

    ks_zip_compress(data, strlen(data) + 1, &sz);
}

void ks_utest_zip_test()
{
    test_1();
    test_2();
}
