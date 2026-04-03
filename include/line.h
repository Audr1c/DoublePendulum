#ifndef _LINE_H_
#define _LINE_H_

#include "images.h"

void draw_line_thick_smooth(Image img, int x0, int y0, int x1, int y1, Brush b, Pixel color);
void put_pixel(Image img, int x, int y, Pixel color);

Pixel blend_pixel(Pixel dst, Pixel src);

Brush make_circular_brush(int diameter);

// stamp brush with given color
void stamp_brush(Image img, int cx, int cy, Brush b, Pixel color);

void draw_thick_line_fast(Image img, int x0, int y0, int x1, int y1, int thickness, Pixel col);
#endif