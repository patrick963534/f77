#include "utest.h"
#include "tests/zip_test.h"
#include "tests/path_test.h"

void ks_utest_start()
{
    ks_utest_path_test();
    ks_utest_zip_test();
}