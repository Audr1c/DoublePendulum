#ifndef _COLOR_H_
#define _COLOR_H_

#include <math.h>
#include "images.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

typedef struct {
    uint8_t r, g, b;
} RGB;

RGB rgb(uint8_t r, uint8_t g, uint8_t b);

Pixel rgbTopix(RGB rgb, uint8_t a);

RGB HSVtoRGB(double H, double S, double V);

typedef struct
{
    double x;       // normalized position 0..1
    double r, g, b; // RGB in [0,1]
} Anchor;

RGB getColor(double t, Anchor *anchors, int n);

Anchor *load_colormap(const char *filename, const char *colormap, int *out_size);

#endif
