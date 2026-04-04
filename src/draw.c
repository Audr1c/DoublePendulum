#include "draw.h"

int draw_pendulums(Image final_img, DoublePen *Dpens, int numPendulums, int sizeBrush)
{
    // setup the OpenCL system
    char *kernel_source = read_kernel_file("src/kernel.cl");
    OCL_System ocl = setup_opencl(final_img->matrix, final_img->count, final_img->width, final_img->height, kernel_source);

    // if (ocl.kernel == NULL)
    // {
        
    // }
    // draw the pendulums
    for (int i = 0; i < numPendulums; i++)
    {
        if (draw_double_pendulum_OCL(&ocl, final_img, Dpens[i], sizeBrush) != 0)
        {
            printf("Error drawing pendulum %d\n", i);
            return 1;
        }
    }

    // cleanup
    fetch_and_average_image(&ocl, final_img);
    int v = blackpixel(final_img);
    if (v >= final_img->width * final_img->height )
    {
        printf("nb of black pixels: %d\n", v);
        // return 1;
    }
    // fetch_and_cleanup(&ocl, final_img->matrix);
    return 0;
}