#include <stdlib.h>
#include "r7_zbuffer.h"

#define swap_ptr(type, p1, p2)  { type* _type_t = p1; p1 = p2; p2 = _type_t; }

#define shift_big(v)    ((v) << 16)
#define shift_small(v)  (((v) + (1 << 15)) >> 16)

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
    int lx = shift_big(vp->x);
    int rx = shift_big(vp->x);
    int lu = (int)(vp->u * shift_big((tex_w)));
    int ru = (int)(vp->u * shift_big((tex_w)));
    int lv = (int)(vp->v * shift_big((tex_h)));
    int rv = (int)(vp->v * shift_big((tex_h)));

    unsigned short* pp = zb->pbuf + zb->xsize * (vp->y);
    unsigned short* texture = zb->current_texture;
    unsigned char*  alpha = zb->alpha;

    int dxl = shift_big(lp->x - vp->x) / (abs(vp->y - lp->y));
    int dxr = shift_big(rp->x - vp->x) / (abs(vp->y - rp->y));

    int dul = (int)((lp->u - vp->u) * shift_big(tex_w) / (abs(vp->y - lp->y)));
    int dur = (int)((rp->u - vp->u) * shift_big(tex_w) / (abs(vp->y - rp->y)));
    int dvl = (int)((lp->v - vp->v) * shift_big(tex_h) / (abs(vp->y - lp->y)));
    int dvr = (int)((rp->v - vp->v) * shift_big(tex_h) / (abs(vp->y - rp->y)));

    int top_flat_right_edge_minor = 0;
    int top_flat_right_edge_y_offset = 0;

    if (line_step < 0)
    {
        top_flat_right_edge_y_offset = -1;
        pp = pp - zb->xsize;
        top_flat_right_edge_minor = -(shift_big(1) >> 1);
    }

    while (nbline-- > 0)
    {
        int n = shift_small(rx + top_flat_right_edge_minor) - shift_small(lx + top_flat_right_edge_minor) + 1;
        int startx = shift_small(lx + top_flat_right_edge_minor);
        unsigned short* line_pp = pp + startx;

        if (n > 0) 
        {
            int tu = lu;
            int tv = lv;
            int tdu = n == 1 ? 0 : (ru - lu) / (n - 1);
            int tdv = n == 1 ? 0 : (rv - lv) / (n - 1);
            unsigned short bv = 0, bb = 0;

            while (n > 0)
            {
                int xx = shift_small(tu);
                int yy = shift_small(tv) + top_flat_right_edge_y_offset;

                if (xx < 0) xx = 0;
                else if (xx >= tex_w) xx = tex_w - 1;

                if (yy < 0) yy = 0;
                else if (yy >= tex_h) yy = tex_h - 1;

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

                        int r = (((dv >> 11) & 0x1F) * (255 - a) + ((sv >> 11) & 0x1F) * a) >> 8;
                        int g = (((dv >>  5) & 0x3F) * (255 - a) + ((sv >>  5) & 0x3F) * a) >> 8;
                        int b = (((dv      ) & 0x1F) * (255 - a) + ((sv      ) & 0x1F) * a) >> 8;

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

                if (bb)
                {
                    *(line_pp-1) = bv;
                    bb = 0;
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

            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y)) - 1;          
            if (nbline > 0 && p0->y == p1->y)
                ++nbline;
        }

        if (nbline > 0)
            draw_part(zb, vp, lp, rp, nbline, part == 0? zb->xsize : -zb->xsize);
    }
}