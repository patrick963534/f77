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

static void draw_part(ZBuffer * zb, ZBufferPoint * vp, ZBufferPoint * lp, ZBufferPoint * rp, int nbline, int line_step ) 
{
    int tex_w = zb->tex_w;
    int tex_h = zb->tex_h;
    float lx = (float)vp->x;
    float rx = (float)vp->x;
    float lu = (float)vp->u * tex_w - 1;
    float ru = (float)vp->u * tex_w - 1;
    float lv = (float)vp->v * tex_h - 1;
    float rv = (float)vp->v * tex_h - 1;

    unsigned short* pp = zb->pbuf + zb->xsize * (vp->y);
    unsigned short* texture = zb->current_texture;
    unsigned char*  alpha = zb->alpha;

    float dxl = (float)(lp->x - vp->x) / (float)(abs(vp->y - lp->y));
    float dxr = (float)(rp->x - vp->x) / (float)(abs(vp->y - rp->y));

    float dul = (float)(lp->u - vp->u) * (tex_w - 1) / (float)(abs(vp->y - lp->y));
    float dur = (float)(rp->u - vp->u) * (tex_w - 1) / (float)(abs(vp->y - rp->y));
    float dvl = (float)(lp->v - vp->v) * (tex_h - 1) / (float)(abs(vp->y - lp->y));
    float dvr = (float)(rp->v - vp->v) * (tex_h - 1) / (float)(abs(vp->y - rp->y));

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
            unsigned short bv = 0, bb = 0;

            while (n > 0)
            {
                int xx = (int)(tu + 0.5f);
                int yy = (int)(tv + 0.5f);

                if (xx >= 0 && yy >= 0 && xx < tex_w && yy < tex_h)
                {
                    int ppidx = yy * tex_w + xx;
                    unsigned char a = 0xff;

                    if (alpha) a = alpha[ppidx];

                    if (a == 0xff)
                    {
                        bv = texture[ppidx];
                    }
                    else if (a > 0)
                    {
                        int sv = texture[ppidx];
                        int dv = *line_pp;
                        float af = a / 255.0f;

                        int r = (int)(((dv >> 11) & 0x1F) * (1- af) + ((sv >> 11) & 0x1F) * af);
                        int g = (int)(((dv >>  5) & 0x3F) * (1- af) + ((sv >>  5) & 0x3F) * af);
                        int b = (int)(((dv      ) & 0x1F) * (1- af) + ((sv      ) & 0x1F) * af);

                        bv = (unsigned short)((r << 11) | (g << 5) | b);
                    }

                    if (bb)
                    {
                        if (a == 0)
                            *(line_pp - 1) = bv;
                        else
                            *((unsigned int*)(line_pp - 1)) = texture[ppidx] << 16 | bv;

                        bb = 0;
                    }

                    if (a)
                        bb = 1;
                }

                tu += tdu;
                tv += tdv;
                ++line_pp;
                --n;
            }

            if (bb)
            {
                *(line_pp-1) = bv;
                bb = 0;
            }
        }

        lx += dxl; rx += dxr;
        lu += dul; ru += dur;
        lv += dvl; rv += dvr;
        pp += line_step;
    }
}

void ZB_rasterize(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    int area;
    ZBufferPoint *vp, *lp, *rp;
    int part;
    int nbline;

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

            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y));
            if (nbline > 0 && p1->y != p2->y)
                ++nbline;
        }
        else
        {
            vp = p2; lp = p0; rp = p1;
            if (area < 0)
                swap_ptr(ZBufferPoint, lp, rp);

            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y));          
            if (nbline > 0 && p0->y == p1->y)
                ++nbline;
        }

        if (nbline > 0)
            draw_part(zb, vp, lp, rp, nbline, part == 0? zb->xsize : -zb->xsize);
    }
}