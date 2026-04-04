#include "mainFunc.h"
// objective is to draw a few line on a 128x128 black image

int main()
{
    Pixel bg = pixel(0, 0, 0, 255);
    Image img = create_blank_image(128, 128, 4, bg);

    char *kernel_source = read_kernel_file("src/kernel.cl");
    OCL_System ocl = setup_opencl(img->matrix,img->count, img->width, img->height, kernel_source);

    // Draw few lines 
    draw_line(&ocl, 0, 0, 128, 128, 6, pixel(255, 0, 0, 128));
    draw_line(&ocl, 25, 25, 75, 75,  10, pixel(0, 255, 0, 150));
    draw_line(&ocl, 0, 0, 0, 128,   5, pixel(0, 0, 255, 50));

    fetch_and_average_image(&ocl, img);
    if (createImage(img, "test", 1415) != 0)
    {
        printf("Error creating image\n");
        return 1;
    }
    return 0;
}