#include <ks/director.h>
#include <ks/sys/system.h>

static ks_director_t* director;

static void director_update()
{

}

static void director_draw()
{

}

KS_API void ks_director_init(const char* title, int w, int h)
{
    director = (ks_director_t*)ks_container_new(sizeof(*director));
    director->width = w;
    director->height = h;

    ks_sys_system_init((ks_container_t*)director);
    ks_sys_system_instance()->klass->create(title, w, h);
}

KS_API void ks_director_run(ks_scene_t* scene)
{
    ks_unused(scene);
}

KS_API ks_director_t* ks_director_instance()
{
    return director;
}