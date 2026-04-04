#include "images.h"


Pixel pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    Pixel p;
    p.r = r;
    p.g = g;
    p.b = b;
    p.a = a;
    return p;
}


Image create_blank_image(int width, int height, int numOfchan, pix bgCol){

    Image img = malloc(sizeof(*img));
    if(img ==NULL){
        fprintf(stderr, "Failed to allocate memory for the image pointer\n");
        return NULL;
    }
   
    img->width = width;
    img->height = height;
    img->numChannel = numOfchan;
    img->matrix = malloc(sizeof(pix) * width* height);
    img->count = malloc(sizeof(int) * width* height);
    
    if (img->matrix == NULL) {
        fprintf(stderr, "Failed to allocate memory for image matrix width of size %d width, %d height\n", width, height);
        return NULL;
    }
    for (int i = 0; i < width; i++){
        // img->matrix[i] = malloc(sizeof(pix) * height);
        // if (img->matrix[i] == NULL)
        // {
        //     fprintf(stderr, "Failed to allocate memory for line in matrix at index [%d]\n", i);
        //     for (int j = 0; j < i; j++)
        //     {
        //         free(img->matrix[j]);
        //     }
        //     return NULL;
        // }
        for (int j = 0; j < height; j++)
        {
            img->matrix[i * width + j] = bgCol;
            img->count[i * width + j] = 1;
        }
    }
    if(img == NULL){
        printf("Failed to create image Fun\n");
    }
    return img;
}

int blackpixel(Image img){
    int count = 0;
    for (int i = 0; i < img->width; i++)
    {
        for (int j = 0; j < img->height; j++)
        {
            pix p = img->matrix[i * img->width + j];
            if (p.r == 0 && p.g == 0 && p.b == 0)
                count++;
            if (img->count[i * img->width + j] != 1)
                count -= 10000000;
        }
    }
    return count;
}
// Image load_image(const char *filename);

int saveImage(Image img, const char *filename)
{
    // lets copy th data to see if it changes anything
    int w = img->width;
    int h = img->height;
    int ch = img->numChannel;
    int total_pixels = w * h;

    uint8_t *buffer = (uint8_t *)malloc(total_pixels * ch);
    if (!buffer || ch != 4)
        return -1;

    // Traitement séparé selon le nombre de canaux
    
    #pragma omp parallel for simd
    for (int i = 0; i < total_pixels; i++)
    {
        int r = img->matrix[i].r;
        int g = img->matrix[i].g;
        int b = img->matrix[i].b;
        int a = img->matrix[i].a;

        buffer[i * 4 + 0] = r < 0 ? 0 : (r > 255 ? 255 : r);
        buffer[i * 4 + 1] = g < 0 ? 0 : (g > 255 ? 255 : g);
        buffer[i * 4 + 2] = b < 0 ? 0 : (b > 255 ? 255 : b);
        buffer[i * 4 + 3] = a < 0 ? 0 : (a > 255 ? 255 : a);
    }
    int v = stbi_write_bmp(filename, w, h, ch, buffer);
    if (v == 0){
        // from source code : stb / stb_image_write.h
        // [ Each function returns 0 on failure and non-0 on success.]
        printf("Image not saved %s, error code: %d\n", filename, v);
        // print a part of the buffer to see if it changes anything
        for (int i = 0; i < 10; i++)
        {
            printf("\t%d %d %d %d\n", buffer[i * 4 + 0], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
        }
        printf("End of buffer\n");
    }
    free(buffer);

    return v == 0 ? -1 : 0;
    // create a new table of pixel (rgba) instead of rgban___
    // uint8_t *table = malloc(w * h * sizeof(*table)* ch);
    // for (int y = 0; y < h; y++)
    // {
    //     for (int x = 0; x < w; x++)
    //     {
    //         int idx = (y * w + x) * ch; // pixel index in array
    //         table[idx + 0] = img->matrix[x + y * w].r;        // R
    //         table[idx + 1] = img->matrix[x + y * w].g;        // G
    //         table[idx + 2] = img->matrix[x + y * w].b;        // B
    //         if (ch == 4)
    //             table[idx + 3] = img->matrix[x + y * w].a; // A (if present)
    //     }
    // }
    // return 0;
    // free(table);
    // stbi_image_free(data);
    // return stbi_write_png(filename, img->width, img->height, img->numChannel, img->matrix, img->width * img->numChannel);
    // unsigned char *data = malloc(w * h * ch);
    // for (int i = 0; i < w ; i++){
    //     for(int j = 0; j < h; j++){
    //         int idx = (j * w + i) * ch; // pixel index in array
    //         data[idx + 0] = img->matrix[i][j].r;        // R
    //         data[idx + 1] = img->matrix[i][j].g;        // G
    //         data[idx + 2] = img->matrix[i][j].b;        // B
    //         if (ch == 4)
    //             data[idx + 3] = img->matrix[i][j].a; // A (if present)
    //     }

    // }

    // stbi_write_png(filename, w, h, ch, img->matrix, w * ch);
}

Image free_image(Image img)
{
    // for (int i = 0; i < img->width; i++)
    // {
    //     free(img->matrix[i]);
    // }
    free(img->count);
    free(img->matrix);
    free(img);
    return NULL;
}
// static inline int mkdir(const char *path)
// {
//     char tmp[512];
//     char *p = NULL;
//     size_t len;

//     snprintf(tmp, sizeof(tmp), "%s", path);
//     len = strlen(tmp);
//     if (tmp[len - 1] == '/')
//         tmp[len - 1] = '\0';

//     for (p = tmp + 1; *p; p++)
//     {
//         if (*p == '/')
//         {
//             *p = '\0';
//             if (mkdir(tmp) && errno != EEXIST)
//                 return -1;
//             *p = '/';
//         }
//     }
//     if (mkdir(tmp) && errno != EEXIST)
//         return -1;
//     return 0;
// }

// static inline int create_directory(const char *path, char **last_slash) {
//     if (last_slash)
//     {
//         *last_slash = '\0'; // remove filename
//         if (mkdir_p(path) != 0)
//         {
//             perror("mkdir_p failed");
//             return 1;
//         }
//     }
//     return 0;
// }

int createImage(Image img, char *folder, int frame)
{
    char filename[128];
    sprintf(filename, "resources/%s/images%d.bmp", folder, frame);
    // char *last_slash = strrchr(filename, '/');
    // create_directory(filename, &last_slash);
    // save image
    if (saveImage(img, filename) != 0)
    {
        fprintf(stderr, "Failed to save image to :\"%s\"n", filename);
        return 1;
    };
    img = free_image(img);
    if (img != NULL)
    {
        fprintf(stderr, "Failed to free image\n");
        return 1;
    }
    return 0;
}