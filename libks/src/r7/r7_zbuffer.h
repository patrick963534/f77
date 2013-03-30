#ifndef _tgl_zbuffer_h_
#define _tgl_zbuffer_h_

/*
 * Z buffer
 */

#include "r7_zfeatures.h"

#define ZB_POINT_RED_MIN ( (1<<10) )
#define ZB_POINT_RED_MAX ( (1<<16)-(1<<10) )
#define ZB_POINT_GREEN_MIN ( (1<<9) )
#define ZB_POINT_GREEN_MAX ( (1<<16)-(1<<9) )
#define ZB_POINT_BLUE_MIN ( (1<<10) )
#define ZB_POINT_BLUE_MAX ( (1<<16)-(1<<10) )

typedef struct {
    int xsize,ysize;
    int linesize; /* line size, in bytes */
    int mode;
    
    unsigned short *zbuf;
    unsigned short *pbuf;
    int frame_buffer_allocated;
    
    int nb_colors;
    unsigned char *dctable;
    int *ctable;
    unsigned short *current_texture;
    int tex_w;
    int tex_h;
} ZBuffer;

typedef struct {
  int x,y,z;     /* integer coordinates in the zbuffer */
  int s,t;       /* coordinates for the mapping */
  int r,g,b;     /* color indexes */

  float u,v;
  
  float sz,tz;   /* temporary coordinates for mapping */
} ZBufferPoint;

/* zbuffer.c */

ZBuffer *ZB_open(int xsize,int ysize,void *frame_buffer);


void ZB_close(ZBuffer *zb);

void ZB_resize(ZBuffer *zb,void *frame_buffer,int xsize,int ysize);
void ZB_clear(ZBuffer *zb,int clear_z,int z,
	      int clear_color,int r,int g,int b);
/* linesize is in BYTES */
void ZB_copyFrameBuffer(ZBuffer *zb,void *buf,int linesize);

/* zdither.c */

void ZB_initDither(ZBuffer *zb,int nb_colors,
		   unsigned char *color_indexes,int *color_table);
void ZB_closeDither(ZBuffer *zb);
void ZB_ditherFrameBuffer(ZBuffer *zb,unsigned char *dest,
			  int linesize);

/* zline.c */

void ZB_plot(ZBuffer *zb,ZBufferPoint *p);
void ZB_line(ZBuffer *zb,ZBufferPoint *p1,ZBufferPoint *p2);
void ZB_line_z(ZBuffer * zb, ZBufferPoint * p1, ZBufferPoint * p2);

/* ztriangle.c */

void ZB_setTexture(ZBuffer *zb, unsigned short *texture, int w, int h);

//void ZB_fillTriangleSmooth(ZBuffer *zb, ZBufferPoint *p1,ZBufferPoint *p2,ZBufferPoint *p3);

//void ZB_fillTriangleMappingPerspective(ZBuffer *zb, ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2);

void ZB_rasterize(ZBuffer *zb, ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2);


typedef void (*ZB_fillTriangleFunc)(ZBuffer  *, ZBufferPoint *,ZBufferPoint *,ZBufferPoint *);

/* memory.c */
void gl_free(void *p);
void *gl_malloc(int size);
void *gl_zalloc(int size);

#endif /* _tgl_zbuffer_h_ */
