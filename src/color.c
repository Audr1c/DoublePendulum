#include "color.h"

RGB rgb(uint8_t r, uint8_t g, uint8_t b){
    RGB p;
    p.r = r;
    p.g = g;
    p.b = b;
    return p;
}

Pixel rgbTopix(RGB rgb, uint8_t a){
    Pixel p;
    p.r = rgb.r;
    p.g = rgb.g;
    p.b = rgb.b;
    p.a = a;
    return p;
}
/* H in degrees (any value, will be wrapped), S and V in [0,1] */
RGB HSVtoRGB(double H, double S, double V){
    RGB p;
    // printf("H: %f, S: %f, V: %f\n", H, S, V);
    /* clamp S,V to [0,1] */
    if (S < 0.0)
        S = 0.0;
    else if (S > 1.0)
        S = 1.0;
    if (V < 0.0)
        V = 0.0;
    else if (V > 1.0)
        V = 1.0;

    if (S == 0.0)
    {
        /* grayscale */
        int vi = (int)(V * 255.0 + 0.5);
        if (vi < 0)
            vi = 0;
        else if (vi > 255)
            vi = 255;
        p.r = p.g = p.b = (uint8_t)vi;
        return p;
    }

    /* wrap H to [0,360) */
    H = fmod(H, 360.0);
    if (H < 0.0)
        H += 360.0;

    double C = V * S;
    double Hp = H / 60.0;
    double X = C * (1.0 - fabs(fmod(Hp, 2.0) - 1.0));
    double r1 = 0.0, g1 = 0.0, b1 = 0.0;

    if (Hp < 1.0)
    {
        r1 = C;
        g1 = X;
        b1 = 0.0;
    }
    else if (Hp < 2.0)
    {
        r1 = X;
        g1 = C;
        b1 = 0.0;
    }
    else if (Hp < 3.0)
    {
        r1 = 0.0;
        g1 = C;
        b1 = X;
    }
    else if (Hp < 4.0)
    {
        r1 = 0.0;
        g1 = X;
        b1 = C;
    }
    else if (Hp < 5.0)
    {
        r1 = X;
        g1 = 0.0;
        b1 = C;
    }
    else
    {
        r1 = C;
        g1 = 0.0;
        b1 = X;
    }

    double m = V - C;
    /* convert to 0..255 with rounding and clamp */
    int ri = (int)((r1 + m) * 255.0 + 0.5);
    int gi = (int)((g1 + m) * 255.0 + 0.5);
    int bi = (int)((b1 + m) * 255.0 + 0.5);
    // printf("r: %d, g: %d, b: %d\n", ri, gi, bi);

    if (ri < 0)
        ri = 0;
    else if (ri > 255)
        ri = 255;
    if (gi < 0)
        gi = 0;
    else if (gi > 255)
        gi = 255;
    if (bi < 0)
        bi = 0;
    else if (bi > 255)
        bi = 255;

    p.r = (uint8_t)ri;
    p.g = (uint8_t)gi;
    p.b = (uint8_t)bi;
    // printf("P   r: %d, g: %d, b: %d\n", p.r, p.g, p.b);
    return p;
}

#include <stdio.h>

typedef struct
{
    double r, g, b;
} Color;

RGB getColor(double t, Anchor *anchors, int n){

    if (t <= anchors[0].x)
        return rgb((uint8_t)255 * anchors[0].r, (uint8_t)255 * anchors[0].g, (uint8_t)255 * anchors[0].b);
    if (t >= anchors[n - 1].x)
        return rgb((uint8_t)255 * anchors[n - 1].r, (uint8_t)255 * anchors[n - 1].g, (uint8_t)255 * anchors[n - 1].b);
    for (int i = 0; i < n - 1; i++)
    {
        if (t >= anchors[i].x && t <= anchors[i + 1].x)
        {
            double u = (t - anchors[i].x) / (anchors[i + 1].x - anchors[i].x);
            return rgb(
                (uint8_t)255 * (anchors[i].r + u * (anchors[i + 1].r - anchors[i].r)),
                (uint8_t)255 * (anchors[i].g + u * (anchors[i + 1].g - anchors[i].g)),
                (uint8_t)255 * (anchors[i].b + u * (anchors[i + 1].b - anchors[i].b)));
        }
    }
    return (RGB){0, 0, 0}; // should not happen
}




Anchor *load_colormap(const char *filename, const char *colormap, int *out_size)
{
    // open file
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        perror("fopen");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data = malloc(len + 1);
    if (!data)
    {
        fclose(f);
        return NULL;
    }
    fread(data, 1, len, f);
    fclose(f);
    data[len] = '\0';

    // parse JSON
    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root)
    {
        fprintf(stderr, "Couldn't parse the json File\n");
        return NULL;
    }

    // get colormap object
    cJSON *cm = cJSON_GetObjectItemCaseSensitive(root, colormap);
    if (!cJSON_IsObject(cm))
    {
        fprintf(stderr, "The Colormap \"%s\"was not found \n", colormap);
        cJSON_Delete(root);
        return NULL;
    }

    // get size
    cJSON *size = cJSON_GetObjectItemCaseSensitive(cm, "size");
    cJSON *string = cJSON_GetObjectItemCaseSensitive(cm, "string");
    if (!cJSON_IsNumber(size) || !cJSON_IsString(string))
    {
        fprintf(stderr, "Parameters Size or string weren't found\n");
        cJSON_Delete(root);
        return NULL;
    }

    int n = size->valueint;
    *out_size = n;

    // allocate anchors
    Anchor *anchors = malloc(sizeof(Anchor) * n);
    if (!anchors)
    {
        fprintf(stderr, "Anchor allocation faield \n");
        cJSON_Delete(root);
        return NULL;
    }

    // parse string: format "{x,r,g,b},{x,r,g,b},..."
    const char *s = string->valuestring;
    for (int i = 0; i < n; i++)
    {
        double x, r, g, b;
        int matched = sscanf(s, " { %lf , %lf , %lf , %lf }", &x, &r, &g, &b);
        if (matched != 4)
        {
            // try without spaces
            matched = sscanf(s, "{%lf,%lf,%lf,%lf}", &x, &r, &g, &b);
        }
        if (matched == 4)
        {
            anchors[i].x = x;
            anchors[i].r = r;
            anchors[i].g = g;
            anchors[i].b = b;
        }
        else
        {
            fprintf(stderr, "parse error at anchor %d\n", i);
            free(anchors);
            cJSON_Delete(root);
            return NULL;
        }
        // move s forward to next '{'
        s = strchr(s + 1, '{');
        if (!s && i < n - 1)
        {
            fprintf(stderr, "unexpected end of string\n");
            free(anchors);
            cJSON_Delete(root);
            return NULL;
        }
    }

    cJSON_Delete(root);
    return anchors;
}
