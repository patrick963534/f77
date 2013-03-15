#include <stdio.h>
#include <ks/time.h>
#include <ks/log.h>
#include <ks/system.h>
#include <stdlib.h>
#include <string.h>
#include <ks/director.h>
#include <ks/scene.h>
#include <ks/actor.h>
#include <ks/helper.h>
#include <ks/event.h>
#include <ks/text.h>
#include <ks/libc.h>
#include <ks/constants.h>
#include "utest/utest.h"
#include "r7/r7_zbuffer.h"
#include <r7/gl.h>


static void step(ks_node_t* me, int delta)
{
    static float offset = 0;

    offset += delta * 50 / 1000.0f;

    if (offset > 1)
    {
        me->y += (int)offset;
        offset -= (int)offset;
    }
}

static void step_fps(ks_text_t* me, int delta)
{
    static float ellapse = 0;

    ellapse += delta;

    if (ellapse > 1000)
    {
        char buf[128];

        ks_text_set_content(me, ks_itoa(buf, sizeof(buf), ks_director_instance()->fps));
        me->x = me->image->width / 2;
        me->y = ks_director_instance()->height - me->image->height / 2;

        ellapse = 0;
    }    
}

static int msgs(ks_node_t* me, ks_event_t* e)
{
    if (e->type == ks.types.KEY_DOWN)
    {
        if (e->key.code == ks.keys.Up)
            me->y += 20;
        else if (e->key.code == ks.keys.Down)
            me->y -= 20;
        else if (e->key.code == ks.keys.Left)
            me->x -= 20;
        else if (e->key.code == ks.keys.Right)
            me->x += 20;
    }

    return 0;
}

static ks_scene_t* create_scene()
{
    ks_scene_t* me;
    ks_node_t*  node;
//     ks_actor_t* actor0;
//     ks_actor_t* actor1;
//     ks_actor_t* actor2;

    me       = ks_scene_new(sizeof(*me));
    me->msgs = msgs;

    node = ks_node_new(sizeof(*node), (ks_node_t*)me);
    node->x = 100;

    ks_text_new("arial.ttf", 18, 60, "",        node)->step = (ks_step_f)step_fps;
    ks_text_new("arial.ttf", 18, 60, "0",       node)->y = 200;
    ks_text_new("arial.ttf", 18, 60, "00",      node)->y = 60;
    ks_text_new("arial.ttf", 18, 60, "000",     node)->y = 90;
    ks_text_new("arial.ttf", 18, 60, "0000",    node)->y = 120;
    ks_text_new("arial.ttf", 18, 60, "00000",   node)->y = 150;    

//     actor0 = ks_actor_new("img.png", (ks_node_t*)me);
//     actor1 = ks_actor_new("img.png", (ks_node_t*)me);
//     actor2 = ks_actor_new("img.png", (ks_node_t*)me);
// 
//     actor0->x = 256;
//     actor0->y = 256;
//     actor1->x = 400 + 256;
//     actor1->y = 256;
//     actor2->x = 800 + 256;
//     actor2->y = 256;
// 
//     actor0->step = step;

    

    return me;
}

static GLuint make_object( void )
{
    GLuint list;

    list = glGenLists( 1 );

    glNewList( list, GL_COMPILE );

    glBegin( GL_LINE_LOOP );
        glVertex3f(  1.0f,  0.5f, 0.4f );
        glVertex3f(  1.0f, -0.5f, 0.4f );
        glVertex3f( -1.0f, -0.5f, 0.4f );
        glVertex3f( -1.0f,  0.5f, 0.4f );
    glEnd();

    glEndList();

    return list;
}

static void test()
{
    ZBuffer *zb;
    char* buf;
    int width = 320;
    int height = 240;
    GLuint Object;

    buf = calloc(1, width * height * 4);
    zb = ZB_open(width, height, ZB_MODE_RGB24, 0, NULL, NULL, NULL);

    glInit(zb);
    glViewport(0, 0, width, height);
    ZB_resize(zb, buf, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
    glMatrixMode(GL_MODELVIEW);

    Object = make_object();

    glClear( GL_COLOR_BUFFER_BIT );
    glPushMatrix();
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -10.0f );
    glColor3f( 1.0, 1.0, 1.0 );
    glCallList( Object );
    glPopMatrix();
    glFlush();

    ks_helper_image_save_ppm("ab.ppm", buf, width, height);    
}

int main(int argc, char** argv)
{
    ks_director_init("Hello World", 320, 240, argc, argv);

    ks_utest_start();

    test();
    ks_director_run(create_scene());
    return 0;
}
