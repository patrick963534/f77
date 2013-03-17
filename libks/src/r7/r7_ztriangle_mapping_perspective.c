#include <stdlib.h>
#include "r7_zbuffer.h"

#define ZCMP(z,zpix) ((z) >= (zpix))

#define NB_INTERP 8

static PIXEL *texture;
static float fdzdx,fndzdx,ndszdx,ndtzdx;

static ZBufferPoint *zpt,*pr1=NULL,*pr2=NULL,*l1 = NULL,*l2 = NULL;
static float fdx1, fdx2, fdy1, fdy2, fz;
static unsigned short *pz1;
static PIXEL *pp1;
static int part,update_left,update_right;

static int nb_lines,dx1,dy1,tmp,dx2,dy2;

static int error=0,derror=0;
static int x1=0,dxdy_min=0,dxdy_max=0;
/* warning: x2 is multiplied by 2^16 */
static int x2=0,dx2dy2=0;  

static int z1=0,dzdx=0,dzdy=0,dzdl_min=0,dzdl_max=0;

static float sz1=0,dszdx=0,dszdy=0,dszdl_min=0,dszdl_max=0;
static float tz1=0,dtzdx=0,dtzdy=0,dtzdl_min=0,dtzdl_max=0;

#define TEXTURE_PIXEL(texture, t, s)  \
            (((char *)texture + (((t & 0x3FC00000) | (s & 0x003FC000)) >> (17 - PSZSH))))

static void DRAW_LINE()				
{						
    register unsigned short *pz;		
    register PIXEL *pp;		
    register unsigned int s,t,z,zz;	
    register int n,dsdx,dtdx;		
    float sz,tz,fz,zinv; 

    n = (x2>>16) - x1;                             
    fz = (float)z1;
    zinv = 1.0f / fz;
    pp = (PIXEL *)((char *)pp1 + x1 * PSZB); 
    pz = pz1 + x1;					
    z = z1;						
    sz = sz1;
    tz = tz1;

    while (n >= (NB_INTERP-1))    
    {						   
        float ss,tt;
        int i;
        ss = (sz * zinv);
        tt = (tz * zinv);
        s = (int) ss;
        t = (int) tt;
        dsdx = (int)((dszdx - ss*fdzdx)*zinv);
        dtdx = (int)((dtzdx - tt*fdzdx)*zinv);
        fz += fndzdx;
        zinv = 1.0f / fz;
        
        for (i = 0; i < 8; ++i)
        {
            zz=z >> ZB_POINT_Z_FRAC_BITS;		
            if (ZCMP(zz, pz[i])) 
            {				
                pp[i]=*(PIXEL *)TEXTURE_PIXEL(texture, t, s);
                pz[i]=(unsigned short)zz;				
            }		
            z+=dzdx; s+=dsdx; t+=dtdx;
        }

        pz+=NB_INTERP;							   
        pp=(PIXEL *)((char *)pp + NB_INTERP * PSZB);
        n-=NB_INTERP;							   
        sz+=ndszdx;
        tz+=ndtzdx;
    }									   
    {
        float ss,tt;
        ss = (sz * zinv);
        tt = (tz * zinv);
        s = (int) ss;
        t = (int) tt;
        dsdx = (int)( (dszdx - ss*fdzdx)*zinv );
        dtdx = (int)( (dtzdx - tt*fdzdx)*zinv );
    }
    while (n>=0) 
    {							   
        zz=z >> ZB_POINT_Z_FRAC_BITS;		
        if (ZCMP(zz, pz[0])) 
        {				
            pp[0]=*(PIXEL *)TEXTURE_PIXEL(texture, t, s);
            pz[0]=(unsigned short)zz;				
        }		
        z+=dzdx; s+=dsdx; t+=dtdx;

        pz += 1;								   
        pp=(PIXEL *)((char *)pp + PSZB);
        n-=1;								   
    }									   
}

void ZB_fillTriangleMappingPerspective(ZBuffer *zb, ZBufferPoint *p0,ZBufferPoint *p1,ZBufferPoint *p2)
{
    /* we sort the vertex with increasing y */
    if (p1->y < p0->y) {
        zpt = p0;
        p0 = p1;
        p1 = zpt;
    }
    if (p2->y < p0->y) {
        zpt = p2;
        p2 = p1;
        p1 = p0;
        p0 = zpt;
    } else if (p2->y < p1->y) {
        zpt = p1;
        p1 = p2;
        p2 = zpt;
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

    {
        float zz, d1, d2;

        d1 = (float)(p1->z - p0->z);
        d2 = (float)(p2->z - p0->z);
        dzdx = (int) (fdy2 * d1 - fdy1 * d2);
        dzdy = (int) (fdx1 * d2 - fdx2 * d1);

        zz=(float) p0->z;
        p0->sz= (float) p0->s * zz;
        p0->tz= (float) p0->t * zz;
        zz=(float) p1->z;
        p1->sz= (float) p1->s * zz;
        p1->tz= (float) p1->t * zz;
        zz=(float) p2->z;
        p2->sz= (float) p2->s * zz;
        p2->tz= (float) p2->t * zz;

        d1 = p1->sz - p0->sz;
        d2 = p2->sz - p0->sz;
        dszdx = (fdy2 * d1 - fdy1 * d2);
        dszdy = (fdx1 * d2 - fdx2 * d1);

        d1 = p1->tz - p0->tz;
        d2 = p2->tz - p0->tz;
        dtzdx = (fdy2 * d1 - fdy1 * d2);
        dtzdy = (fdx1 * d2 - fdx2 * d1);
    }

    /* screen coordinates */

    pp1 = (PIXEL *) ((char *) zb->pbuf + zb->linesize * p0->y);
    pz1 = zb->zbuf + p0->y * zb->xsize;

    texture=zb->current_texture;
    fdzdx=(float)dzdx;
    fndzdx=NB_INTERP * fdzdx;
    ndszdx=NB_INTERP * dszdx;
    ndtzdx=NB_INTERP * dtzdx;

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

            sz1=l1->sz;
            dszdl_min=(dszdy + dszdx * dxdy_min);
            dszdl_max=dszdl_min + dszdx;

            tz1=l1->tz;
            dtzdl_min=(dtzdy + dtzdx * dxdy_min);
            dtzdl_max=dtzdl_min + dtzdx;
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

                sz1+=dszdl_max;
                tz1+=dtzdl_max;
            } else {
                x1+=dxdy_min;
                z1+=dzdl_min;

                sz1+=dszdl_min;
                tz1+=dtzdl_min;
            } 

            /* right edge */
            x2+=dx2dy2;

            /* screen coordinates */
            pp1=(PIXEL *)((char *)pp1 + zb->linesize);
            pz1+=zb->xsize;
        }
    }
}