#include <stdlib.h>
#include "r7_zbuffer.h"

static void sort_point_by_y(ZBufferPoint** zp0, ZBufferPoint** zp1, ZBufferPoint** zp2)
{
    ZBufferPoint *p0, *p1, *p2, *t;
    p0 = *zp0; p1 = *zp1; p2 = *zp2;

    if (p1->y < p0->y)
    {
        t = p0; p0 = p1; p1 = t;
    }

    if (p2->y < p0->y)
    {
        t = p2; p2 = p1; p1 = p0; p0 = t;
    }
    else if (p2->y < p1->y)
    {
        t = p2; p2 = p1; p1 = t;
    }

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

    float invslope1 = (float)(p1->x - p0->x) / (float)(p1->y - p0->y);
    float invslope2 = (float)(p2->x - p0->x) / (float)(p2->y - p0->y);

    float curx1 = (float)p0->x;
    float curx2 = (float)p0->x;

    for (y = p0->y; y <= p1->y; y++)
    {
        draw_line(zb, curx1, curx2, y);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

static void fill_top_flat_triangle(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    int y;

    float invslope1 = (float)(p2->x - p0->x) / (float)(p2->y - p0->y);
    float invslope2 = (float)(p2->x - p1->x) / (float)(p2->y - p1->y);

    float curx1 = (float)p2->x;
    float curx2 = (float)p2->x;

    for (y = p2->y; y > p0->y; y--)
    {
        curx1 -= invslope1;
        curx2 -= invslope2;
        draw_line(zb, curx1, curx2, y);
    }
}

void ZB_fillTriangleMappingPerspective(ZBuffer *zb, ZBufferPoint *p0, ZBufferPoint *p1, ZBufferPoint *p2)
{
    if (calculate_area(p0, p1, p2) == 0)
        return;

    sort_point_by_y(&p0, &p1, &p2);    

    if (p0->y == p1->y)
    {
        --p0->y;
        --p1->y;
        fill_top_flat_triangle(zb, p0, p1, p2);
        ++p0->y;
        ++p1->y;
    }
    else if (p1->y == p2->y)
    {
        fill_bottom_flat_triangle(zb, p0, p1, p2);
    }
    else
    {
        ZBufferPoint p;
        p.x = (int)((p1->y - p0->y) * (float)(p2->x - p0->x) / (float)(p2->y - p0->y)) + p0->x;
        p.y = p1->y;

        fill_bottom_flat_triangle(zb, p0, &p, p1);
        fill_top_flat_triangle(zb, p1, &p, p2);
    }


}