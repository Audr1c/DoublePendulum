#include <stdio.h>
#include <stdlib.h>

#include "line.h"
#include "color.h"
#include "images.h"
#include "pendulum.h"
#include "timing.h"

int firstTest()
{
    int w, h, ch;
    unsigned char *data = stbi_load("resources/input.png", &w, &h, &ch, 0);
    if (!data)
    {
        printf("Failed to load image\n");
        return 1;
    }

    // Example: set pixel at (10,10) to red
    int x = 10, y = 10;
    int idx = (y * w + x) * ch; // pixel index in array
    data[idx + 0] = 255;        // R
    data[idx + 1] = 0;          // G
    data[idx + 2] = 0;          // B
    if (ch == 4)
        data[idx + 3] = 255; // A (if present)

    stbi_write_png("resources/modified.png", w, h, ch, data, w * ch);
    stbi_image_free(data);

    // Create blank white image 100x100 with 3 channels (RGB)
    w = 100;
    h = 100;
    ch = 3;
    unsigned char *blank = malloc(w * h * ch);
    for (int i = 0; i < w * h * ch; i++)
        blank[i] = 255; // white

    stbi_write_png("resources/blank.png", w, h, ch, blank, w * ch);
    free(blank);
    return 0;
}

int test_Lines()
{

    // create a blank white image
    Pixel bg = pixel(255, 255, 255, 255);
    Image img = create_blank_image(100, 100, 4, bg);
    if (img == NULL)
    {
        fprintf(stderr, "Failed to create image\n");
        return 1;
    }

    Brush b9 = make_circular_brush(9);
    Brush b6 = make_circular_brush(6);
    Brush b2 = make_circular_brush(2);

    Pixel red = pixel(255, 0, 0, 10);
    Pixel green = pixel(0, 255, 0, 180);
    Pixel blue = pixel(0, 0, 255, 200);
    Pixel black = pixel(0, 0, 0, 40);

    draw_line_thick_smooth(img, 10, 10, 50, 50, b9, red);
    draw_line_thick_smooth(img, 50, 50, 10, 50, b6, green);
    draw_line_thick_smooth(img, 70, 90, 90, 10, b2, blue);
    draw_line_thick_smooth(img, 20, 50, 70, 70, b9, black);

    if (saveImage(img, "resources/lines.png") != 0)
    {
        fprintf(stderr, "Failed to save image\n");
        return 1;
    };
    img = free_image(img);
    return 0;
}
