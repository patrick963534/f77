#include "path_test.h"
#include <ks/path.h>
#include <ks/log.h>
#include <string.h>

static void path_basename_test()
{
    char buf[7];

    ks_path_basename(buf, sizeof(buf), "c:/aaa/bbb/dd.ext");
    
    if (strcmp(buf, "dd.ext") != 0)
    {
        ks_log("fail utest for ks_path_base_name method.");
    }
}

static void path_folder_test()
{
    char buf[16];

    ks_path_folder(buf, sizeof(buf), "c:/aaa/bbb/dd.ext");

    if (strcmp(buf, "c:/aaa/bbb") != 0)
    {
        ks_log("fail utest for ks_path_base_name method.");
    }
}

static void path_extension_test()
{
    char buf[16];

    ks_path_extention(buf, sizeof(buf), "c:/aaa/bbb/dd.ext");

    if (strcmp(buf, ".ext") != 0)
    {
        ks_log("fail utest for ks_path_base_name method.");
    }
}

static void path_without_ext_test()
{
    char buf[16];

    ks_path_without_ext(buf, sizeof(buf), "c:/aaa/bbb/dd.ext");

    if (strcmp(buf, "c:/aaa/bbb/dd") != 0)
    {
        ks_log("fail utest for ks_path_base_name method.");
    }
}

static void path_join_test()
{
    char buf[16];

    ks_path_join(buf, sizeof(buf), "c:/aa", "bb.ext");

    if (strcmp(buf, "c:/aa/bb.ext") != 0)
    {
        ks_log("fail utest for ks_path_base_name method.");
    }
}

void ks_utest_path_test()
{
    path_basename_test();
    path_folder_test();
    path_extension_test();
    path_without_ext_test();
    path_join_test();
}