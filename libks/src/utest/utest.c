#include "utest.h"
#include "tests/zip_test.h"
#include "tests/path_test.h"
#include "tests/image_test.h"
#include "tests/r7_test.h"
#include "tests/sl_test.h"

void ks_utest_start()
{
    //ks_utest_path_test();
    //ks_utest_zip_test();
    //ks_utest_image_test();
    ks_utest_r7_test();
    ks_utest_sl_test();
}
