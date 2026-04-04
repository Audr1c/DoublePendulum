#include "line.h"

// clamp helper
static inline uint8_t clamp255(int v)
{
    return v < 0 ? 0 : (v > 255 ? 255 : v);
}

// blend: src over dst
static inline Pixel blend(Pixel dst, Pixel src)
{
    float alpha = src.a / 255.0f;
    Pixel out;
    out.r = clamp255((int)(src.r * alpha + dst.r * (1 - alpha)));
    out.g = clamp255((int)(src.g * alpha + dst.g * (1 - alpha)));
    out.b = clamp255((int)(src.b * alpha + dst.b * (1 - alpha)));
    out.a = clamp255((int)(src.a + dst.a * (1 - alpha)));
    return out;
}


// draw blended pixel safely
void put_pixel(Image img, int x, int y, Pixel color)
{
    if (x >= 0 && x < img->width && y >= 0 && y < img->height)
    {
        img->matrix[x + img->width * y] = blend(img->matrix[x + img->width * y], color);
    }
}

Brush make_circular_brush(int diameter)
{
    Brush b;
    b.size = diameter;
    int r = diameter / 2;
    float radius = r + 0.5f; // soft edge
    for (int y = 0; y < diameter; y++)
    {
        for (int x = 0; x < diameter; x++)
        {
            float dx = x - r;
            float dy = y - r;
            float dist = sqrtf(dx * dx + dy * dy);
            float a = 1.0f - (dist - (radius - 1));
            if (dist > radius)
                a = 0;
            if (a < 0)
                a = 0;
            if (a > 1)
                a = 1;
            b.alpha[y][x] = (uint8_t)(a * 255);
        }
    }
    return b;
}

void stamp_brush(Image img, int cx, int cy, Brush b, Pixel color)
{
    int r = b.size / 2;
    for (int y = 0; y < b.size; y++)
    {
        for (int x = 0; x < b.size; x++)
        {
            if (b.alpha[y][x] > 0)
            {
                Pixel col = color;
                col.a = (col.a * b.alpha[y][x]) / 255;
                put_pixel(img, cx + x - r, cy + y - r, col);
            }
        }
    }
}

Pixel blend_pixel(Pixel dst, Pixel src)
{
    uint8_t sa = src.a;
    uint8_t ia = 255 - sa;
    dst.r = (src.r * sa + dst.r * ia) / 255;
    dst.g = (src.g * sa + dst.g * ia) / 255;
    dst.b = (src.b * sa + dst.b * ia) / 255;
    dst.a = clamp255(sa + (dst.a * ia) / 255);
    return dst;
}
static inline Pixel blend_over(Pixel dst, Pixel src)
{
    uint8_t sa = src.a, ia = 255 - sa;
    Pixel out;
    out.r = (uint8_t)((src.r * sa + dst.r * ia) / 255);
    out.g = (uint8_t)((src.g * sa + dst.g * ia) / 255);
    out.b = (uint8_t)((src.b * sa + dst.b * ia) / 255);
    out.a = clamp255(sa + (dst.a * ia) / 255);
    return out;
}

// draw thick smooth line
void draw_line_thick_smooth(Image img, int x0, int y0, int x1, int y1, Brush b, Pixel color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1)
    {
        stamp_brush(img, x0, y0, b, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void draw_thick_line_fast(Image img, int x0, int y0, int x1, int y1, int thickness, Pixel col)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int r = thickness / 2;
    int r2 = r * r;
    while (1)
    {
        for (int yy = -r; yy <= r; yy++)
        {
            for (int xx = -r; xx <= r; xx++)
            {
                if (xx * xx + yy * yy <= r2 && x0 + xx >= 0 && x0 + xx < img->width && y0 + yy >= 0 && y0 + yy < img->height){
                    img->matrix[x0 + xx + img->width *(y0 + yy)] = blend_pixel(img->matrix[x0 + xx + img->width *(y0 + yy)], col);
                }
            }
        }
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}