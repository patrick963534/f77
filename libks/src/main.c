#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>
#include <ks/system.h>
#include <stdlib.h>
#include <string.h>
#include <ks/director.h>
#include <ks/scene.h>
#include <ks/zip.h>

int main()
{
    const char* data = "adsfjoaewijfaowegjagael;fjeiowfoasdfaeoifjawoegj;eig";
    int ret = 0;
    ks_zip_compress(data, strlen(data) + 1, &ret);

    ks_director_init("Hello World", 320, 240);
    ks_director_run(ks_scene_new(sizeof(ks_scene_t)));
    return 0;
}
