#include <stdlib.h>
#include "r7_zbuffer.h"

#define ZCMP(z,zpix) ((z) >= (zpix))

void ZB_fillTriangleFlat(ZBuffer *zb,
			 ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
#if TGL_FEATURE_RENDER_BITS == 24
    unsigned char colorR, colorG, colorB;
#else
    int color;
#endif

#define INTERP_Z

#if TGL_FEATURE_RENDER_BITS == 24 

#define DRAW_INIT()				\
{						\
  colorR=p2->r>>8; \
  colorG=p2->g>>8; \
  colorB=p2->b>>8; \
}

#define PUT_PIXEL(_a)		\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      pp[3 * _a]=(PIXEL)colorR;\
      pp[3 * _a + 1]=(PIXEL)colorG;\
      pp[3 * _a + 2]=(PIXEL)colorB;\
      pz[_a]=(unsigned short)zz;				\
    }\
    z+=dzdx;					\
}

#else

#define DRAW_INIT()				\
{						\
  color=RGB_TO_PIXEL(p2->r,p2->g,p2->b);	\
}
  
#define PUT_PIXEL(_a)				\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
      pp[_a]=(PIXEL)color;				\
      pz[_a]=(unsigned short)zz;				\
    }						\
    z+=dzdx;					\
}
#endif /* TGL_FEATURE_RENDER_BITS == 24 */

#include "r7_ztriangle.h"
}

void ZB_setTexture(ZBuffer *zb,PIXEL *texture)
{
    zb->current_texture=texture;
}

void ZB_fillTriangleMapping(ZBuffer *zb,
			    ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    PIXEL *texture;

#define INTERP_Z
#define INTERP_ST

#define DRAW_INIT()				\
{						\
  texture=zb->current_texture;			\
}

#if TGL_FEATURE_RENDER_BITS == 24

#define PUT_PIXEL(_a)				\
{						\
   unsigned char *ptr;\
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       ptr = texture + (((t & 0x3FC00000) | s) >> 14) * 3; \
       pp[3 * _a]= (PIXEL)ptr[0];\
       pp[3 * _a + 1]= (PIXEL)ptr[1];\
       pp[3 * _a + 2]= (PIXEL)ptr[2];\
       pz[_a]=(unsigned short)zz;				\
    }						\
    z+=dzdx;					\
    s+=dsdx;					\
    t+=dtdx;					\
}

#else

#define PUT_PIXEL(_a)				\
{						\
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       pp[_a]=(PIXEL)texture[((t & 0x3FC00000) | s) >> 14];	\
       pz[_a]=(unsigned short)zz;				\
    }						\
    z+=dzdx;					\
    s+=dsdx;					\
    t+=dtdx;					\
}

#endif

#include "r7_ztriangle.h"
}