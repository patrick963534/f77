#include "path_test.h"
#include <ks/image.h>
#include <ks/log.h>
#include <ks/helper.h>
#include <string.h>
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
#include "../../r7/r7_zbuffer.h"
#include <r7/gl.h>

static GLuint TexObj[2];
static GLfloat Angle = 0.0f;

static int cnt=0,v=0;

static void bind_texture(int texobj,int image)
{
    static int width = 8, height = 8;
    static GLubyte color[2][3]={
        {255,0,0},
        {0,255,0},
    };
    GLubyte tex[64][3];
    static GLubyte texchar[2][8*8] = {
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
        },
        {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 2, 2, 0, 0, 0,
            0, 0, 2, 0, 0, 2, 0, 0,
            0, 0, 0, 0, 0, 2, 0, 0,
            0, 0, 0, 0, 2, 0, 0, 0,
            0, 0, 0, 2, 0, 0, 0, 0,
            0, 0, 2, 2, 2, 2, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
        },
    };

    int i,j;

    glBindTexture(GL_TEXTURE_2D, texobj);

    /* red on white */
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int p = i * width + j;
            if (texchar[image][(height - i - 1) * width + j]) {
                tex[p][0] = color[image][0];
                tex[p][1] = color[image][1];
                tex[p][2] = color[image][2];
            } else {
                tex[p][0] = 255;
                tex[p][1] = 255;
                tex[p][2] = 255;
            }
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* end of texture object */
}

static void smooth_test()
{
    ZBuffer *zb;
    char* buf;
    int width = 320;
    int height = 240;
    GLuint Object;

    buf = calloc(1, width * height * 2);
    zb = ZB_open(width, height, 0);

    glInit(zb);
    glViewport(0, 0, width, height);
    ZB_resize(zb, buf, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
    glMatrixMode(GL_MODELVIEW);

    Object = glGenLists(1);
    glNewList(Object, GL_COMPILE);
    glBegin(GL_TRIANGLES);
        glColor3f( 1.0, 1.0, 0.0 ); glVertex3f(  0.0f,  0.5f, 0.4f );
        glColor3f( 1.0, 0.0, 1.0 ); glVertex3f(  0.5f, -0.5f, 0.4f );
        glColor3f( 0.0, 1.0, 1.0 ); glVertex3f( -0.5f, -0.5f, 0.4f );
    glEnd();
    glEndList();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);
    glCallList(Object);
    glPopMatrix();
    glFlush();

    ks_helper_image565_save_ppm("smooth_test.ppm", buf, width, height);

    free(buf);
    glClose();
    ZB_close(zb);
}

static void texture_mapping_test()
{
    ZBuffer *zb;
    char* buf;
    int width = 100;
    int height = 100;

    buf = calloc(1, width * height * 2);
    zb = ZB_open(width, height, 0);

    glInit(zb);
    glViewport(0, 0, width, height);
    ZB_resize(zb, buf, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 15.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);

    /* Setup texturing */
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    /* generate texture object IDs */
    glGenTextures(2, TexObj);
    bind_texture(TexObj[0],0);
    bind_texture(TexObj[1],1);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0, 0, -5);
    glBegin(GL_TRIANGLES);
        //glTexCoord2f(0.0, 0.0); glVertex3f( 0.0f, 0.5f, 0.f);
        //glTexCoord2f(1.0, 0.0); glVertex3f( 0.5f, 0.5f, 0.f);
        //glTexCoord2f(1.0, 1.0); glVertex3f( 0.5f, 0.0f, 0.f);

        //glTexCoord2f(0.0, 0.0); glVertex3f( 0.0f,  0.5f, 0.f);
        //glTexCoord2f(0.0, 1.0); glVertex3f( 0.0f,  0.0f, 0.f);
        //glTexCoord2f(1.0, 1.0); glVertex3f( 0.5f,  0.0f, 0.f);

         glTexCoord2f(0.0, 0.0); glVertex3f( 0.0f,  0.5f, 0.f);
         glTexCoord2f(1.0, 0.0); glVertex3f( 0.5f, -0.5f, 0.f);
         glTexCoord2f(1.0, 1.0); glVertex3f(-0.5f, -0.5f, 0.f);

//         glTexCoord2f(1.0, 1.0); glVertex3f(-0.5f, -0.8f, 0.f);
//         glTexCoord2f(0.0, 0.0); glVertex3f( 0.0f,  0.5f, 0.f);
//         glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f,  0.5f, 0.f);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(2, TexObj);

    glFlush();

    ks_helper_image565_save_ppm("texture_mapping_test.ppm", buf, width, height);

    free(buf);
    glClose();
    ZB_close(zb);
}

void ks_utest_r7_test()
{
    //smooth_test();
    texture_mapping_test();
}
