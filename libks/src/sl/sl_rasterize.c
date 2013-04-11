#include "sl.c.h"
#include <sl/sl.h>
#include <math.h>
#include <stdlib.h>

#define swap_ptr(type, p1, p2)  { type* _type_t = p1; p1 = p2; p2 = _type_t; }

#define shift_big(v)    ((v) << 16)
#define shift_small(v)  (((v) + (1 << 15)) >> 16)

static void sort_point_by_y(sl_zb_point_t** zp0, sl_zb_point_t** zp1, sl_zb_point_t** zp2)
{
    sl_zb_point_t *p0, *p1, *p2;
    p0 = *zp0; p1 = *zp1; p2 = *zp2;

    if (p1->y < p0->y)
        swap_ptr(sl_zb_point_t, p0, p1);

    if (p2->y < p0->y)
    {
        swap_ptr(sl_zb_point_t, p2, p1);
        swap_ptr(sl_zb_point_t, p1, p0);
    }
    else if (p2->y < p1->y)
        swap_ptr(sl_zb_point_t, p2, p1);

    *zp0 = p0; *zp1 = p1; *zp2 = p2;
}

static int calculate_area(sl_zb_point_t *p0, sl_zb_point_t *p1, sl_zb_point_t *p2)
{
    int dx1 = p1->x - p0->x;
    int dy1 = p1->y - p0->y;
    int dx2 = p2->x - p0->x;
    int dy2 = p2->y - p0->y;

    return dx1 * dy2 - dx2 * dy1;
}

static void draw_part(sl_zbuffer_t * zb, sl_zb_point_t * vp, sl_zb_point_t * lp, sl_zb_point_t * rp, int nbline, int line_step ) 
{
    int tex_w = zb->tex_w;
    int tex_h = zb->tex_h;
    int lx = shift_big(vp->x);
    int rx = shift_big(vp->x);
    int lu = (int)(vp->u * shift_big((tex_w)));
    int ru = (int)(vp->u * shift_big((tex_w)));
    int lv = (int)(vp->v * shift_big((tex_h)));
    int rv = (int)(vp->v * shift_big((tex_h)));

    uint* pp = zb->pbuf + zb->w * (vp->y);
    uint* texture = zb->texture;

    int dxl = shift_big(lp->x - vp->x) / (abs(vp->y - lp->y));
    int dxr = shift_big(rp->x - vp->x) / (abs(vp->y - rp->y));

    int dul = (int)((lp->u - vp->u) * shift_big(tex_w) / (abs(vp->y - lp->y)));
    int dur = (int)((rp->u - vp->u) * shift_big(tex_w) / (abs(vp->y - rp->y)));
    int dvl = (int)((lp->v - vp->v) * shift_big(tex_h) / (abs(vp->y - lp->y)));
    int dvr = (int)((rp->v - vp->v) * shift_big(tex_h) / (abs(vp->y - rp->y)));

    if (line_step < 0)
    {
        lx += dxl; rx += dxr;
        lu += dul; ru += dur;
        lv += dvl; rv += dvr;
        pp += line_step;
    }

    while (nbline-- > 0)
    {
        int startx = shift_small(lx) + 1;
        int n = shift_small(rx) - startx + 1;
        uint* line_pp = pp + startx;

        if (n > 0) 
        {
            int tu = lu;
            int tv = lv;
            int tdu = (ru - lu) / (n); // because 'startx' already add '1', here 'n' don't need to add '-1' again.
            int tdv = (rv - lv) / (n); // because 'startx' already add '1', here 'n' don't need to add '-1' again.

            while (n > 0)
            {
                int xx = shift_small(tu);
                int yy = shift_small(tv);
                uchar a;
                uint sv;

                if (xx < 0) xx = 0; else if (xx >= tex_w) xx = tex_w - 1;
                if (yy < 0) yy = 0; else if (yy >= tex_h) yy = tex_h - 1;

                sv = texture[yy * tex_w + xx];
                a = (sv >> 24) & 0xFF;

                if (a == 0xff)
                {
                    *line_pp = sv;
                }
                else if (a > 0)
                {
                    int dv = *line_pp;

                    uchar r = (((dv >> 16) & 0xFF) * (255 - a) + ((sv >> 16) & 0xFF) * a) >> 8;
                    uchar g = (((dv >>  8) & 0xFF) * (255 - a) + ((sv >>  8) & 0xFF) * a) >> 8;
                    uchar b = (((dv      ) & 0xFF) * (255 - a) + ((sv      ) & 0xFF) * a) >> 8;

                    *line_pp = 0xFF000000 | r << 16 | g << 8 | b;
                }

                tu += tdu;
                tv += tdv;
                ++line_pp;
                --n;
            }
        }

        lx += dxl; rx += dxr;
        lu += dul; ru += dur;
        lv += dvl; rv += dvr;
        pp += line_step;
    }
}

void sl_rasterize(sl_zb_point_t* p0, sl_zb_point_t* p1, sl_zb_point_t* p2)
{
    sl_zbuffer_t* zb = &sl_context()->zbuffer;
    int area;
    sl_zb_point_t *vp, *lp, *rp;
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
                swap_ptr(sl_zb_point_t, lp, rp);

            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y));
            if (nbline > 0 && p1->y != p2->y)
                ++nbline;
        }
        else
        {
            vp = p2; lp = p0; rp = p1;
            if (area < 0)
                swap_ptr(sl_zb_point_t, lp, rp);

            nbline = min(abs(vp->y - lp->y), abs(vp->y - rp->y)) - 1;          
            if (nbline > 0 && p0->y == p1->y)
                ++nbline;
        }

        if (nbline > 0)
            draw_part(zb, vp, lp, rp, nbline, part == 0? zb->w : -zb->w);
    }
}

SL_API void sl_texture_load(int w, int h, uint* pixels)
{
    sl_context_t* c = sl_context();

    c->zbuffer.tex_w = w;
    c->zbuffer.tex_h = h;
    c->zbuffer.texture = pixels;
}