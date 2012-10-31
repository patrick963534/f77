#include <ks/zip.h>
#include <stdlib.h>
#include <string.h>

static void test_1()
{
    char* data;
    int ret, sz, i;

    sz = 65536;
    data = malloc(sz);

    srand(0);

    for (i = 0; i < sz; i++)
        data[i] = rand() % 26;

    ks_zip_compress(data, sz, &ret);
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