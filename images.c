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
        }
    }
    if(img == NULL){
        printf("Failed to create image Fun\n");
    }
    return img;
}



//Image load_image(const char *filename);

int saveImage(Image img, const char *filename){
    // lets copy th data to see if it changes anything
    int w = img->width;
    int h = img->height;
    int ch = img->numChannel;
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
    stbi_write_bmp(filename, w, h, ch, img->matrix);
    // stbi_image_free(data);
    return 0;
    //return stbi_write_png(filename, img->width, img->height, img->numChannel, img->matrix, img->width * img->numChannel);
}

Image free_image(Image img){
    // for (int i = 0; i < img->width; i++)
    // {
    //     free(img->matrix[i]);
    // }
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
    //create_directory(filename, &last_slash);
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