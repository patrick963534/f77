#include <stdlib.h>
#include "r7_zbuffer.h"

#define swap_ptr(type, p1, p2)  { type* _type_t = p1; p1 = p2; p2 = _type_t; }

static void sort_point_by_y(ZBufferPoint** zp0, ZBufferPoint** zp1, ZBufferPoint** zp2)
{
    ZBufferPoint *p0, *p1, *p2;
    p0 = *zp0; p1 = *zp1; p2 = *zp2;

    if (p1->y < p0->y)
        swap_ptr(ZBufferPoint, p0, p1);

    if (p2->y < p0->y)
    {
        swap_ptr(ZBufferPoint, p2, p1);
        swap_ptr(ZBufferPoint, p1, p0);
    }
    else if (p2->y < p1->y)
        swap_ptr(ZBufferPoint, p2, p1);

    *zp0 = p0; *zp1 = p1; *zp2 = p2;
}

static int calculate_area(ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    int dx1 = p1->x - p0->x;
    int dy1 = p1->y - p0->y;
    int dx2 = p2->x - p0->x;
    int dy2 = p2->y - p0->y;

    return dx1 * dy2 - dx2 * dy1;
}

static void draw_line(ZBuffer *zb, float x1, float x2, int y)
{
    unsigned short* pp = zb->pbuf + zb->xsize * y;
    int n;

    if (x1 > x2) 
    {
        float t = x2; 
        x2 = x1; 
        x1 = t;
    }

    n = (int)x2 - (int)x1;
    pp = pp + (int)(x1);

    while (n-- >= 0)
    {
        *pp++ = 0xffff;
    }
}

void ZB_fillTriangleMappingPerspective(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    int area;
    ZBufferPoint *vp, *lp, *rp;
    int part;
    int nbline;
    int line_step;

    sort_point_by_y(&p0, &p1, &p2);   

    if ((area = calculate_area(p0, p1, p2)) == 0)
        return;

    for (part = 0; part < 2; ++part)
    {
        if (part == 0)
        {
            vp = p0; lp = p2; rp = p1;
            if (area < 0)
                swap_ptr(ZBufferPoint, lp, rp);

            line_step = zb->xsize;
            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y)) + 1;
        }
        else
        {
            vp = p2; lp = p0; rp = p1;
            if (area < 0)
                swap_ptr(ZBufferPoint, lp, rp);

            line_step = -zb->xsize;
            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y));          
            if (p0->y == p1->y)
                ++nbline;
        }

        if (nbline <= 0)
            continue;

        {
            int tex_w = 256;
            int tex_h = 256;
            float lx = (float)vp->x;
            float rx = (float)vp->x;
            float lu = (float)vp->u * tex_w;
            float ru = (float)vp->u * tex_w;
            float lv = (float)vp->v * tex_h;
            float rv = (float)vp->v * tex_h;

            unsigned short* pp = zb->pbuf + zb->xsize * (vp->y);
            unsigned short* texture = zb->current_texture;

            float dxl = (float)(lp->x - vp->x) / (float)(abs(vp->y - lp->y));
            float dxr = (float)(rp->x - vp->x) / (float)(abs(vp->y - rp->y));

            float dul = (float)(lp->u - vp->u) * tex_w / (float)(abs(vp->y - lp->y));
            float dur = (float)(rp->u - vp->u) * tex_w / (float)(abs(vp->y - rp->y));
            float dvl = (float)(lp->v - vp->v) * tex_h / (float)(abs(vp->y - lp->y));
            float dvr = (float)(rp->v - vp->v) * tex_h / (float)(abs(vp->y - rp->y));

            while (nbline-- > 0)
            {
                int n = (int)(rx + 0.5f) - (int)(lx + 0.5f) + 1;
                unsigned short* line_pp = pp + (int)(lx + 0.5f);

                if (line_step < 0)
                    --n;

                if (n > 0) 
                {
                    float tu = lu;
                    float tv = lv;
                    float tdu = (ru - lu) / n;
                    float tdv = (rv - lv) / n;

                    while (n-- > 0)
                    {
                        int xx = (int)(tu+0.5f);
                        int yy = (int)(tv+0.5f);
                        *line_pp++ = texture[yy * tex_w + xx];

                        //if (part == 0)
                        //    *line_pp++ = 0xff00;
                        //else
                        //    *line_pp++ = 0xffff;


                        tu += tdu;
                        tv += tdv;
                    }
                }

                lx += dxl; rx += dxr;
                lu += dul; ru += dur;
                lv += dvl; rv += dvr;
                pp += line_step;
            }
        }        
    }
}