#include <stdio.h>
#include <stdlib.h>
#include "r7_zbuffer.h"

#if 0

#define ZCMP(z,zpix) ((z) >= (zpix))
#define SAR_RND_TO_ZERO(v,n) (v / (1<<n))

#define PUT_PIXEL(_a)				\
{						\
    zz=z >> ZB_POINT_Z_FRAC_BITS;		\
    if (ZCMP(zz,pz[_a])) {				\
        tmp=rgb & 0xF81F07E0;			\
        pp[_a]=(PIXEL)(tmp | (tmp >> 16));			\
        pz[_a]=(unsigned short)zz;				\
    }						\
    z+=dzdx;					\
    rgb=(rgb+drgbdx) & ( ~ 0x00200800);		\
}

#define DRAW_LINE()							   \
{									   \
  register unsigned short *pz;					   \
  register PIXEL *pp;					   \
  register unsigned int tmp,z,zz,rgb,drgbdx;				   \
  register int n;							   \
  n=(x2 >> 16) - x1;							   \
  pp=pp1+x1;								   \
  pz=pz1+x1;								   \
  z=z1;									   \
  rgb=(r1 << 16) & 0xFFC00000;						   \
  rgb|=(g1 >> 5) & 0x000007FF;						   \
  rgb|=(b1 << 5) & 0x001FF000;						   \
  drgbdx=_drgbdx;							   \
  while (n>=3) {							   \
      PUT_PIXEL(0);							   \
      PUT_PIXEL(1);							   \
      PUT_PIXEL(2);							   \
      PUT_PIXEL(3);							   \
      pz+=4;								   \
      pp+=4;								   \
      n-=4;								   \
  }									   \
  while (n>=0) {							   \
      PUT_PIXEL(0);							   \
      pz+=1;								   \
      pp+=1;								   \
      n-=1;								   \
  }									   \
}


void ZB_fillTriangleSmooth(ZBuffer *zb, ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    int _drgbdx;

    ZBufferPoint *t,*pr1=NULL,*pr2=NULL,*l1 = NULL,*l2 = NULL;
    float fdx1, fdx2, fdy1, fdy2, fz, d1, d2;
    unsigned short *pz1;
    PIXEL *pp1;
    int part,update_left,update_right;

    int nb_lines,dx1,dy1,tmp,dx2,dy2;

    int error=0,derror=0;
    int x1=0,dxdy_min=0,dxdy_max=0;
    /* warning: x2 is multiplied by 2^16 */
    int x2=0,dx2dy2=0;  

    int z1=0,dzdx=0,dzdy=0,dzdl_min=0,dzdl_max=0;
    int r1=0,drdx=0,drdy=0,drdl_min=0,drdl_max=0;
    int g1=0,dgdx=0,dgdy=0,dgdl_min=0,dgdl_max=0;
    int b1=0,dbdx=0,dbdy=0,dbdl_min=0,dbdl_max=0;

    /* we sort the vertex with increasing y */
    if (p1->y < p0->y) {
        t = p0;
        p0 = p1;
        p1 = t;
    }
    if (p2->y < p0->y) {
        t = p2;
        p2 = p1;
        p1 = p0;
        p0 = t;
    } else if (p2->y < p1->y) {
        t = p1;
        p1 = p2;
        p2 = t;
    }

    /* we compute dXdx and dXdy for all interpolated values */

    fdx1 = (float)(p1->x - p0->x);
    fdy1 = (float)(p1->y - p0->y);

    fdx2 = (float)(p2->x - p0->x);
    fdy2 = (float)(p2->y - p0->y);

    fz = fdx1 * fdy2 - fdx2 * fdy1;
    if (fz == 0)
        return;
    fz = 1.0f / fz;

    fdx1 *= fz;
    fdy1 *= fz;
    fdx2 *= fz;
    fdy2 *= fz;

    d1 = (float)(p1->z - p0->z);
    d2 = (float)(p2->z - p0->z);
    dzdx = (int) (fdy2 * d1 - fdy1 * d2);
    dzdy = (int) (fdx1 * d2 - fdx2 * d1);

    d1 = (float)(p1->r - p0->r);
    d2 = (float)(p2->r - p0->r);
    drdx = (int) (fdy2 * d1 - fdy1 * d2);
    drdy = (int) (fdx1 * d2 - fdx2 * d1);

    d1 = (float)(p1->g - p0->g);
    d2 = (float)(p2->g - p0->g);
    dgdx = (int) (fdy2 * d1 - fdy1 * d2);
    dgdy = (int) (fdx1 * d2 - fdx2 * d1);

    d1 = (float)(p1->b - p0->b);
    d2 = (float)(p2->b - p0->b);
    dbdx = (int) (fdy2 * d1 - fdy1 * d2);
    dbdy = (int) (fdx1 * d2 - fdx2 * d1);

    /* screen coordinates */

    pp1 = (PIXEL *) ((char *) zb->pbuf + zb->linesize * p0->y);
    pz1 = zb->zbuf + p0->y * zb->xsize;

    _drgbdx=(SAR_RND_TO_ZERO(drdx,6) << 22) & 0xFFC00000;		
    _drgbdx|=SAR_RND_TO_ZERO(dgdx,5) & 0x000007FF;		
    _drgbdx|=(SAR_RND_TO_ZERO(dbdx,7) << 12) & 0x001FF000; 	

    for(part=0;part<2;part++) {
        if (part == 0) {
            if (fz > 0) {
                update_left=1;
                update_right=1;
                l1=p0;
                l2=p2;
                pr1=p0;
                pr2=p1;
            } else {
                update_left=1;
                update_right=1;
                l1=p0;
                l2=p1;
                pr1=p0;
                pr2=p2;
            }
            nb_lines = p1->y - p0->y;
        } else {
            /* second part */
            if (fz > 0) {
                update_left=0;
                update_right=1;
                pr1=p1;
                pr2=p2;
            } else {
                update_left=1;
                update_right=0;
                l1=p1; 
                l2=p2;
            }
            nb_lines = p2->y - p1->y + 1;
        }

        /* compute the values for the left edge */

        if (update_left) {
            dy1 = l2->y - l1->y;
            dx1 = l2->x - l1->x;
            if (dy1 > 0) 
                tmp = (dx1 << 16) / dy1;
            else
                tmp = 0;
            x1 = l1->x;
            error = 0;
            derror = tmp & 0x0000ffff;
            dxdy_min = tmp >> 16;
            dxdy_max = dxdy_min + 1;

            z1=l1->z;
            dzdl_min=(dzdy + dzdx * dxdy_min); 
            dzdl_max=dzdl_min + dzdx;

            r1=l1->r;
            drdl_min=(drdy + drdx * dxdy_min);
            drdl_max=drdl_min + drdx;

            g1=l1->g;
            dgdl_min=(dgdy + dgdx * dxdy_min);
            dgdl_max=dgdl_min + dgdx;

            b1=l1->b;
            dbdl_min=(dbdy + dbdx * dxdy_min);
            dbdl_max=dbdl_min + dbdx;
        }

        /* compute values for the right edge */

        if (update_right) {
            dx2 = (pr2->x - pr1->x);
            dy2 = (pr2->y - pr1->y);
            if (dy2>0) 
                dx2dy2 = ( dx2 << 16) / dy2;
            else
                dx2dy2 = 0;
            x2 = pr1->x << 16;
        }

        /* we draw all the scan line of the part */

        while (nb_lines>0) {
            nb_lines--;
            DRAW_LINE();

            /* left edge */
            error+=derror;
            if (error > 0) {
                error-=0x10000;

                x1+=dxdy_max;
                z1+=dzdl_max;

                r1+=drdl_max;
                g1+=dgdl_max;
                b1+=dbdl_max;

            } else {
                x1+=dxdy_min;
                z1+=dzdl_min;

                r1+=drdl_min;
                g1+=dgdl_min;
                b1+=dbdl_min;

            } 

            /* right edge */
            x2+=dx2dy2;

            /* screen coordinates */
            pp1=(PIXEL *)((char *)pp1 + zb->linesize);
            pz1+=zb->xsize;
        }
    }
}

#endif