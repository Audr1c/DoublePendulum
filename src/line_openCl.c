#include "line_openCl.h"

// Fonction utilitaire pour lire le fichier .cl
char *read_kernel_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erreur: Impossible d'ouvrir %s\n", filename);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *source = (char *)malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);
    return source;
}

// 1. Initialisation unique
OCL_System setup_opencl(Pixel *h_img,int *h_count, int img_w, int img_h, const char *kernel_source)
{
    OCL_System ocl;
    ocl.img_w = img_w;
    ocl.img_h = img_h;

    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    ocl.context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    ocl.queue = clCreateCommandQueue(ocl.context, device, 0, NULL);

    ocl.program = clCreateProgramWithSource(ocl.context, 1, &kernel_source, NULL, NULL);
    clBuildProgram(ocl.program, 1, &device, NULL, NULL, NULL);
    ocl.kernel = clCreateKernel(ocl.program, "draw_segment", NULL);

    // Allocation GPU
    size_t img_size = img_w * img_h * sizeof(Pixel);
    ocl.d_img = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, img_size, h_img, NULL);
    size_t count_size = img_w * img_h * sizeof(int);
    ocl.d_count = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, count_size, h_count, NULL);

    // Arguments statiques (ne changent jamais)
    clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), &ocl.d_img);
    clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), &ocl.d_count);
    clSetKernelArg(ocl.kernel, 2, sizeof(int), &ocl.img_w);
    clSetKernelArg(ocl.kernel, 3, sizeof(int), &ocl.img_h);

    return ocl;
}

// 2. Fonction utilitaire de tracé
void draw_line(OCL_System *ocl, int x1, int y1, int x2, int y2, int radius, pix color)
{
    // Mise à jour uniquement des arguments dynamiques
    clSetKernelArg(ocl->kernel, 4, sizeof(int), &x1);
    clSetKernelArg(ocl->kernel, 5, sizeof(int), &y1);
    clSetKernelArg(ocl->kernel, 6, sizeof(int), &x2);
    clSetKernelArg(ocl->kernel, 7, sizeof(int), &y2);
    clSetKernelArg(ocl->kernel, 8, sizeof(int), &radius);
    clSetKernelArg(ocl->kernel, 9, sizeof(int), &color.r);
    clSetKernelArg(ocl->kernel, 10, sizeof(int), &color.g);
    clSetKernelArg(ocl->kernel, 11, sizeof(int), &color.b);
    clSetKernelArg(ocl->kernel, 12, sizeof(int), &color.a);

// Bounding box
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
    int min_x = MAX(0, MIN(x1, x2) - radius);
    int max_x = MIN(ocl->img_w - 1, MAX(x1, x2) + radius);
    int min_y = MAX(0, MIN(y1, y2) - radius);
    int max_y = MIN(ocl->img_h - 1, MAX(y1, y2) + radius);

    size_t global_work_size[2] = {
        (size_t)(max_x - min_x + 1),
        (size_t)(max_y - min_y + 1)};

    // Envoi asynchrone (pas de clFinish ici)
    clEnqueueNDRangeKernel(ocl->queue, ocl->kernel, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
}

// 3. Récupération et nettoyage
void fetch_and_cleanup(OCL_System *ocl, Pixel *h_img, int *h_count)
{
    size_t img_size = ocl->img_w * ocl->img_h * sizeof(Pixel);
    size_t count_size = ocl->img_w * ocl->img_h * sizeof(int);

    // Attend la fin de tous les tracés en attente et lit l'image
    clFinish(ocl->queue);
    clEnqueueReadBuffer(ocl->queue, ocl->d_img, CL_TRUE, 0, img_size, h_img, 0, NULL, NULL);
    clEnqueueReadBuffer(ocl->queue, ocl->d_count, CL_TRUE, 0, count_size, h_count, 0, NULL, NULL);

    clReleaseMemObject(ocl->d_img);
    clReleaseKernel(ocl->kernel);
    clReleaseProgram(ocl->program);
    clReleaseCommandQueue(ocl->queue);
    clReleaseContext(ocl->context);
}

// Récupère l'image depuis le GPU, fait les moyennes et libère OpenCL
void fetch_and_average_image(OCL_System *ocl, Image img)
{
    size_t img_size = img->width * img->height * sizeof(Pixel);
    size_t count_size = img->width * img->height * sizeof(int);

    // 1. Rapatrier les données directement dans le pointeur matrix de votre Image
    clFinish(ocl->queue);
    clEnqueueReadBuffer(ocl->queue, ocl->d_img, CL_TRUE, 0, img_size, img->matrix, 0, NULL, NULL);
    clEnqueueReadBuffer(ocl->queue, ocl->d_count, CL_TRUE, 0, count_size, img->count, 0, NULL, NULL);

    // 2. Parcourir l'image pour faire la moyenne (Division par n)
    int total_pixels = img->width * img->height;
    for (int i = 0; i < total_pixels; i++)
    {
        int count_ = img->count[i];
        if (count_ > 1)
        {
            img->matrix[i].r /= count_;
            img->matrix[i].g /= count_;
            img->matrix[i].b /= count_;
            img->matrix[i].a /= count_;
        }

        // Sécurité : forcer n=1 pour que l'image soit réutilisable normalement ensuite
        if (count_ > 0)
        {
            img->count[i] = 1;
        }
    }

    // 3. Nettoyer OpenCL
    clReleaseMemObject(ocl->d_img);
    clReleaseKernel(ocl->kernel);
    clReleaseProgram(ocl->program);
    clReleaseCommandQueue(ocl->queue);
    clReleaseContext(ocl->context);
}
