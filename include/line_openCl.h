#ifndef _LINE_OPENCL_H_
#define _LINE_OPENCL_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CL/cl.h>
#include "images.h"
#include "line.h"
#include "color.h"
#include "timing.h"


// Structure pour conserver l'état OpenCL entre les appels
typedef struct
{
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_img;
    int img_w;
    int img_h;
} OCL_System;


char *read_kernel_file(const char *filename);
OCL_System setup_opencl(Pixel *h_img, int img_w, int img_h, const char *kernel_source);
void draw_line(OCL_System *ocl, int x1, int y1, int x2, int y2, int radius, pix color);
void fetch_and_cleanup(OCL_System *ocl, Pixel *h_img);
void fetch_and_average_image(OCL_System *ocl, Image img);



#endif