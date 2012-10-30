#include <ks/zip.h>
#include <stdlib.h>

static void test_1()
{
    char* data;
    int ret, sz, i;

    sz = 65536;
    data = malloc(sz);

    srand(0);

    for (i = 0; i < sz; i++)
        data[i] = rand() % 26;

    ks_zip_compress(data, 256, &ret);
}

void ks_utest_zip_test()
{
    test_1();
}