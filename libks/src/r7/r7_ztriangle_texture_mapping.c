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

static void fill_bottom_flat_triangle(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    int y;

    float dx1 = (float)(p0->x - p1->x) / (float)(p0->y - p1->y);
    float dx2 = (float)(p0->x - p2->x) / (float)(p0->y - p2->y);

    float curx1 = (float)p0->x;
    float curx2 = (float)p0->x;

    for (y = p0->y; y <= p1->y; y++)
    {
        draw_line(zb, curx1, curx2, y);
        curx1 += dx1;
        curx2 += dx2;
    }
}

static void fill_top_flat_triangle(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    int y;

    float dx1 = (float)(p2->x - p0->x) / (float)(p2->y - p0->y);
    float dx2 = (float)(p2->x - p1->x) / (float)(p2->y - p1->y);

    float curx1 = (float)p2->x;
    float curx2 = (float)p2->x;

    for (y = p2->y; y > p0->y; y--)
    {
        curx1 -= dx1;
        curx2 -= dx2;
        draw_line(zb, curx1, curx2, y);
    }
}

static void test(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    unsigned short* pp;
    int area;
    ZBufferPoint *vp, *lp, *rp;
    float dxl, dxr;
    int part;

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
        }
        else
        {
            vp = p2; lp = p0; rp = p1;
            if (area < 0)
                swap_ptr(ZBufferPoint, lp, rp);
        }

        {
            float lx = (float)vp->x;
            float rx = (float)vp->x;
            int nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y));
            int line_step = (part == 0) ? zb->xsize : -zb->xsize;

            pp = zb->pbuf + zb->xsize * vp->y;

            dxl = (float)(lp->x - vp->x) / (float)(abs(vp->y - lp->y));
            dxr = (float)(rp->x - vp->x) / (float)(abs(vp->y - rp->y));

            while (nbline-- >= 0)
            {
                int n = (int)rx - (int)lx;
                unsigned short* line_pp = pp + (int)(lx);

                while (n-- >= 0)
                {
                    if (part == 0)
                        *line_pp++ = 0xff00;
                    else
                        *line_pp++ = 0xffff;
                }

                lx += dxl;
                rx += dxr;
                pp += line_step;
            }
        }        
    }
}

void ZB_fillTriangleMappingPerspective(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    test(zb, p0, p1, p2);
//     if (calculate_area(p0, p1, p2) == 0)
//         return;
// 
//     sort_point_by_y(&p0, &p1, &p2);    
// 
//     if (p0->y == p1->y)
//     {
//         --p0->y;
//         --p1->y;
//         fill_top_flat_triangle(zb, p0, p1, p2);
//         ++p0->y;
//         ++p1->y;
//     }
//     else if (p1->y == p2->y)
//     {
//         fill_bottom_flat_triangle(zb, p0, p1, p2);
//     }
//     else
//     {
//         ZBufferPoint p;
//         p.x = (int)((p1->y - p0->y) * (float)(p2->x - p0->x) / (float)(p2->y - p0->y)) + p0->x;
//         p.y = p1->y;
// 
//         fill_bottom_flat_triangle(zb, p0, &p, p1);
//         fill_top_flat_triangle(zb, p1, &p, p2);
//     }


}