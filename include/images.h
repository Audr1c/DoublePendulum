#ifndef _IMAGES_H_
#define _IMAGES_H_

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <string.h>
#include <stdio.h>
// #include <errno.h>

// #define STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

// typedef struct
// {
//     int r, g, b, a, n;
//     int padding[3];
// } pix, Pixel;

// Image

// Pixel
typedef struct
{
    int r, g, b, a; // before uint_8
} pix, Pixel;
typedef struct
{
    int width, height, numChannel;
    pix *matrix;
    int *count;
} _img, *Image;

#define MAX_BRUSH 64
// circular alpha brush
typedef struct
{
    int size;
    uint8_t alpha[MAX_BRUSH][MAX_BRUSH];
} Brush;

Pixel pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);



Image create_blank_image(int width, int height, int numOfchan, pix bgCol);

Image load_image(const char *filename);

int saveImage(Image img, const char *filename);

Image free_image(Image img);

int createImage(Image img, char *folder, int frame);

int blackpixel(Image img);

#endif