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

/*
 * Texture mapping with perspective correction.
 * We use the gradient method to make less divisions.
 * TODO: pipeline the division
 */
#if 1

void ZB_fillTriangleMappingPerspective(ZBuffer *zb,
                            ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    PIXEL *texture;
    float fdzdx,fndzdx,ndszdx,ndtzdx;

#define INTERP_Z
#define INTERP_STZ

#define NB_INTERP 8

#define DRAW_INIT()				\
{						\
  texture=zb->current_texture;\
  fdzdx=(float)dzdx;\
  fndzdx=NB_INTERP * fdzdx;\
  ndszdx=NB_INTERP * dszdx;\
  ndtzdx=NB_INTERP * dtzdx;\
}


#if TGL_FEATURE_RENDER_BITS == 24

#define PUT_PIXEL(_a)				\
{						\
   unsigned char *ptr;\
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       ptr = texture + (((t & 0x3FC00000) | (s & 0x003FC000)) >> 14) * 3;\
       pp[3 * _a]= ptr[0];\
       pp[3 * _a + 1]= ptr[1];\
       pp[3 * _a + 2]= ptr[2];\
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
       pp[_a]=*(PIXEL *)((char *)texture+ \
               (((t & 0x3FC00000) | (s & 0x003FC000)) >> (17 - PSZSH)));\
       pz[_a]=(unsigned short)zz;				\
    }						\
    z+=dzdx;					\
    s+=dsdx;					\
    t+=dtdx;					\
}

#endif

#define DRAW_LINE()				\
{						\
  register unsigned short *pz;		\
  register PIXEL *pp;		\
  register unsigned int s,t,z,zz;	\
  register int n,dsdx,dtdx;		\
  float sz,tz,fz,zinv; \
  n=(x2>>16)-x1;                             \
  fz=(float)z1;\
  zinv=1.0f / fz;\
  pp=(PIXEL *)((char *)pp1 + x1 * PSZB); \
  pz=pz1+x1;					\
  z=z1;						\
  sz=sz1;\
  tz=tz1;\
  while (n>=(NB_INTERP-1)) {						   \
    {\
      float ss,tt;\
      ss=(sz * zinv);\
      tt=(tz * zinv);\
      s=(int) ss;\
      t=(int) tt;\
      dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
      dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
      fz+=fndzdx;\
      zinv=1.0f / fz;\
    }\
    PUT_PIXEL(0);							   \
    PUT_PIXEL(1);							   \
    PUT_PIXEL(2);							   \
    PUT_PIXEL(3);							   \
    PUT_PIXEL(4);							   \
    PUT_PIXEL(5);							   \
    PUT_PIXEL(6);							   \
    PUT_PIXEL(7);							   \
    pz+=NB_INTERP;							   \
    pp=(PIXEL *)((char *)pp + NB_INTERP * PSZB);\
    n-=NB_INTERP;							   \
    sz+=ndszdx;\
    tz+=ndtzdx;\
  }									   \
    {\
      float ss,tt;\
      ss=(sz * zinv);\
      tt=(tz * zinv);\
      s=(int) ss;\
      t=(int) tt;\
      dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
      dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
    }\
  while (n>=0) {							   \
    PUT_PIXEL(0);							   \
    pz+=1;								   \
    pp=(PIXEL *)((char *)pp + PSZB);\
    n-=1;								   \
  }									   \
}
  
#include "r7_ztriangle.h"
}

#endif

#if 0

/* slow but exact version (only there for reference, incorrect for 24
   bits) */

void ZB_fillTriangleMappingPerspective(ZBuffer *zb,
                            ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    PIXEL *texture;

#define INTERP_Z
#define INTERP_STZ

#define DRAW_INIT()				\
{						\
  texture=zb->current_texture;			\
}

#define PUT_PIXEL(_a)				\
{						\
   float zinv; \
   int s,t; \
   zz=z >> ZB_POINT_Z_FRAC_BITS;		\
     if (ZCMP(zz,pz[_a])) {				\
       zinv= 1.0 / (float) z; \
       s= (int) (sz * zinv); \
       t= (int) (tz * zinv); \
       pp[_a]=(PIXEL)texture[((t & 0x3FC00000) | s) >> 14];	\
       pz[_a]=(unsigned short)zz;				\
    }						\
    z+=dzdx;					\
    sz+=dszdx;					\
    tz+=dtzdx;					\
}

#include "ztriangle.h"
}


#endif
