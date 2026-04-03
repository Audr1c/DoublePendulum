#include "mainFunc.h"
/*

int animation_one_simple_pendulum(int numFrames, Measurement m)
{
    int imageWidth = 500;
    int imageHeight = 500;

    Brush b = make_circular_brush(9);

    Pixel red = pixel(255, 0, 0, 60);
    // create a simple pendulum
    // Meas 1
    startMeasurement(m, 1, 0);
    SimplePen pen = create_simple_pendulum(imageWidth / 2, imageHeight / 2, M_PI / 2, 0, 150, 1, red);
    if (pen == NULL)
    {
        fprintf(stderr, "Failed to create pendulum\n");
        return 1;
    }
    printf("Starting animation of simple pendulum\n");
    endMeasurement(m, 1, 0);
    for (int i = 0; i < numFrames; i++)
    {

        printf("\rcurruntely at frame %d (perc : %.2f%%)", i, i / (float)numFrames * 100);
        // printf("\n\t\t");
        // print_simple_pendulum(pen);
        // printf("\n");
        // create a blank black image
        // Meas 2
        startMeasurement(m, 2, i);
        Pixel bg = pixel(0, 0, 0, 255);
        Image img = create_blank_image(imageWidth, imageHeight, 4, bg);
        endMeasurement(m, 2, i);
        if (img == NULL)
        {
            fprintf(stderr, "Failed to create image\n");
            return 1;
        }

        // Meas 3
        startMeasurement(m, 3, i);
        // if (draw_simple_pendulum(img, pen, b) != 0){
        //     fprintf(stderr, "Failed to draw pendulum\n");
        //     return 1;
        // }
        endMeasurement(m, 3, i);

        // Meas 4
        startMeasurement(m, 4, i);
        char filename[128];
        sprintf(filename, "resources/animationSimplePendulum/simplependulum%d.png", i);
        // save image
        if (saveImage(img, filename) != 0)
        {
            fprintf(stderr, "Failed to save image\n");
            return 1;
        };
        img = free_image(img);
        endMeasurement(m, 4, i);

        // Meas 5
        startMeasurement(m, 5, i);
        pen = update_simple_pendulum(pen, 0.1);
        endMeasurement(m, 5, i);
        // Meas 6
        startMeasurement(m, 6, i);
        // pen = update_simple_color(pen);
        endMeasurement(m, 6, i);
        if (pen == NULL)
        {
            fprintf(stderr, "Failed to update pendulum\n");
            return 1;
        }
    }
    pen = free_simple_pendulum(pen);
    printf("\n");
    return 0;
}

int animation_lots_simple_pendulum(int numFrames, int numPendulums, Measurement m)
{
    int imageWidth = 2500;
    int imageHeight = 2500;

    clock_t start = clock();
    clock_t all = clock();
    clock_t end = clock();

    Brush b = make_circular_brush(7);

    // create a simple pendulum
    // Meas 1
    startMeasurement(m, 1, 0);

    SimplePen *pens = malloc(numPendulums * sizeof(SimplePen));

    if (pens == NULL)
    {
        fprintf(stderr, "Failed to create pendulum\n");
        return 1;
    }
    for (int i = 0; i < numPendulums; i++)
    {

        Pixel color = pixel(i * 255 / numPendulums, 255, 0, 10);
        double theta = M_PI / 2 + i * 2 * M_PI / numPendulums;
        while (theta > M_PI)
        {
            theta -= 2 * M_PI;
        }
        while (theta < -M_PI)
        {
            theta += 2 * M_PI;
        }
        SimplePen pen = create_simple_pendulum(imageWidth / 2, imageHeight / 2, theta, 0, 1000, 1, color);
        if (pen == NULL)
        {
            fprintf(stderr, "Failed to create pendulum\n");
            return 1;
        }
        pens[i] = pen;
    }
    printf("Starting animation of simple pendulum\n");
    endMeasurement(m, 1, 0);
    for (int i = 0; i < numFrames; i++)
    {
        end = clock();
        printf("\rcurruntely at frame %d (perc : %.2f%%), Time Elapsed : ", i, i / (float)numFrames * 100);
        printf("    Time Elapsed : ");
        Timeprint((double)(end - all) / CLOCKS_PER_SEC);
        printf(",   Time per frame : ");
        Timeprint((double)(end - start) / CLOCKS_PER_SEC);
        printf(",   Remaing time : ");
        Timeprint((double)(numFrames - i) * (double)(end - start) / CLOCKS_PER_SEC);
        start = clock();
        // printf("\n\t\t");
        // print_simple_pendulum(pen);
        // printf("\n");
        // create a blank black image
        // Meas 2
        startMeasurement(m, 2, i);
        Pixel bg = pixel(0, 0, 0, 255);
        Image img = create_blank_image(imageWidth, imageHeight, 4, bg);
        endMeasurement(m, 2, i);
        if (img == NULL)
        {
            fprintf(stderr, "Failed to create image\n");
            return 1;
        }

        // Meas 3
        startMeasurement(m, 3, i);
        // Measurement mcat3 = create_measurement(numPendulums, 3);
        for (int j = 0; j < numPendulums; j++)
        {
            if (draw_simple_pendulum(img, pens[j], b.size) != 0)
            {
                fprintf(stderr, "Failed to draw pendulum\n");
                return 1;
            }
        }
        // printMeasurement(mcat3);
        // free_measurement(mcat3);
        // exit(0);
        endMeasurement(m, 3, i);

        // Meas 4
        startMeasurement(m, 4, i);
        char filename[128];
        sprintf(filename, "resources/animatioPendulumsBmp/lotssimplependulum%d.bmp", i);
        // save image
        if (saveImage(img, filename) != 0)
        {
            fprintf(stderr, "Failed to save image\n");
            return 1;
        };
        img = free_image(img);
        endMeasurement(m, 4, i);

        // Meas 5
        startMeasurement(m, 5, i);
        for (int j = 0; j < numPendulums; j++)
        {
            pens[j] = update_simple_pendulum(pens[j], 0.4);
            if (pens == NULL)
            {
                fprintf(stderr, "Failed to update pendulum\n");
                return 1;
            }
        }
        endMeasurement(m, 5, i);
        // Meas 6
        startMeasurement(m, 6, i);
        for (int j = 0; j < numPendulums; j++)
        {
            // printf("\n");
            // print_simple_pendulum(pens[j]);
            // printf("\n");
            pens[j] = update_simple_color(pens[j]);
            // print_simple_pendulum(pens[j]);
            // printf("\n");
            // printf("----\n");
        }
        endMeasurement(m, 6, i);
    }
    for (int j = 0; j < numPendulums; j++)
    {
        pens[j] = free_simple_pendulum(pens[j]);
    }
    free(pens);
    printf("\n");
    return 0;
}

int circleOfSimplePendulums(int numFrames, int numPendulums, Measurement m, int skipFrames)
{
    int imageWidth = 1000;
    int imageHeight = 1000;

    clock_t start = clock();
    clock_t all = clock();
    clock_t end = clock();

    Brush b = make_circular_brush(7);

    // create a simple pendulum
    // Meas 1
    startMeasurement(m, 1, 0);

    SimplePen *pens = malloc(numPendulums * sizeof(SimplePen));
    SimplePen *drawingPen = malloc(numPendulums * sizeof(SimplePen));

    if (pens == NULL)
    {
        fprintf(stderr, "Failed to create pendulum\n");
        return 1;
    }
    for (int i = 0; i < numPendulums; i++)
    {

        Pixel color = pixel(i * 255 / numPendulums, 255, 0, 10);
        double theta = M_PI / 2 + i * 2 * M_PI / numPendulums;
        while (theta > M_PI)
        {
            theta -= 2 * M_PI;
        }
        while (theta < -M_PI)
        {
            theta += 2 * M_PI;
        }
        SimplePen drpen = create_simple_pendulum(imageWidth / 2, imageHeight / 2, theta, 0, 400, 1, color);
        SimplePen pen = create_simple_pendulum(imageWidth / 2, imageHeight / 2, theta, 0, 1000, 1, color);
        if (pen == NULL || drawingPen == NULL)
        {
            fprintf(stderr, "Failed to create pendulum\n");
            return 1;
        }
        drawingPen[i] = drpen;
        pens[i] = pen;
    }
    printf("Starting animation of simple pendulum\n");
    endMeasurement(m, 1, 0);
    for (int i = 0; i < numFrames; i++)
    {
        end = clock();
        printf("\rcurruntely at frame %d (perc : %.2f%%), Time Elapsed : ", i, i / (float)numFrames * 100);
        printf("    Time Elapsed : ");
        Timeprint((double)(end - all) / CLOCKS_PER_SEC);
        printf(",   Time per frame : ");
        Timeprint((double)(end - start) / CLOCKS_PER_SEC);
        printf(",   Remaing time : ");
        Timeprint((double)(numFrames - i) * (double)(end - start) / CLOCKS_PER_SEC);
        start = clock();
        // printf("\n\t\t");
        // print_simple_pendulum(pen);
        // printf("\n");
        // create a blank black image
        // Meas 2
        if (i >= skipFrames)
        {
            startMeasurement(m, 2, i);
            Pixel bg = pixel(0, 0, 0, 255);
            Image img = create_blank_image(imageWidth, imageHeight, 4, bg);
            endMeasurement(m, 2, i);
            if (img == NULL)
            {
                fprintf(stderr, "Failed to create image\n");
                return 1;
            }
            // Meas 3
            startMeasurement(m, 3, i);
            // Measurement mcat3 = create_measurement(numPendulums, 3);
            for (int j = 0; j < numPendulums; j++)
            {
                if (draw_simple_pendulum(img, drawingPen[j], b.size) != 0)
                {
                    fprintf(stderr, "Failed to draw pendulum\n");
                    return 1;
                }
            }
            // printMeasurement(mcat3);
            // free_measurement(mcat3);
            // exit(0);
            endMeasurement(m, 3, i);

            // Meas 4
            startMeasurement(m, 4, i);
            char filename[128];
            sprintf(filename, "resources/circleOfSimplePendulums/images%d.bmp", i);
            // save image
            if (saveImage(img, filename) != 0)
            {
                fprintf(stderr, "Failed to save image\n");
                return 1;
            };
            img = free_image(img);
            endMeasurement(m, 4, i);
        }
        // Meas 5
        startMeasurement(m, 5, i);
        for (int j = 0; j < numPendulums; j++)
        {
            pens[j] = update_simple_pendulum(pens[j], .5);
            if (pens == NULL)
            {
                fprintf(stderr, "Failed to update pendulum\n");
                return 1;
            }
        }
        endMeasurement(m, 5, i);
        // Meas 6
        startMeasurement(m, 6, i);
        for (int j = 0; j < numPendulums; j++)
        {
            pens[j] = update_simple_color(pens[j]);
            drawingPen[j]->color = pens[j]->color;
        }
        endMeasurement(m, 6, i);
    }
    for (int j = 0; j < numPendulums; j++)
    {
        pens[j] = free_simple_pendulum(pens[j]);
        drawingPen[j] = free_simple_pendulum(drawingPen[j]);
    }
    free(pens);
    free(drawingPen);
    printf("\n");
    return 0;
}

int circleOfSimplePendulumsMultiThread(int numFrames, int numPendulums, Measurement m, int skipFrames, int numThreads)
{
    int imageWidth = 1000;
    int imageHeight = 1000;

    clock_t start = clock();
    clock_t all = clock();
    clock_t end = clock();

    Brush b = make_circular_brush(7);

    // create a simple pendulum
    // Meas 1
    startMeasurement(m, 1, 0);

    SimplePen *pens = malloc(numPendulums * sizeof(SimplePen));
    SimplePen *drawingPen = malloc(numPendulums * sizeof(SimplePen));

    if (pens == NULL)
    {
        fprintf(stderr, "Failed to create pendulum\n");
        return 1;
    }
    for (int i = 0; i < numPendulums; i++)
    {

        Pixel color = pixel(i * 255 / numPendulums, 255, 0, 10);
        double theta = M_PI / 2 + i * 2 * M_PI / numPendulums;
        while (theta > M_PI)
        {
            theta -= 2 * M_PI;
        }
        while (theta < -M_PI)
        {
            theta += 2 * M_PI;
        }
        SimplePen drpen = create_simple_pendulum(imageWidth / 2, imageHeight / 2, theta, 0, 400, 1, color);
        SimplePen pen = create_simple_pendulum(imageWidth / 2, imageHeight / 2, theta, 0, 450, 1, color);
        if (pen == NULL || drawingPen == NULL)
        {
            fprintf(stderr, "Failed to create pendulum\n");
            return 1;
        }
        drawingPen[i] = drpen;
        pens[i] = pen;
    }
    for (int j = 0; j < numPendulums; j++)
    {
        pens[j] = update_simple_color(pens[j]);
        drawingPen[j]->color = pens[j]->color;
    }
    endMeasurement(m, 1, 0);
    printf("Starting animation of simple pendulum\n");
    for (int i = 0; i < numFrames; i++)
    {
        end = clock();
        printf("\rcurruntely at frame %d (perc : %.2f%%), Time Elapsed : ", i, i / (float)numFrames * 100);
        printf("    Time Elapsed : ");
        Timeprint((double)(end - all) / CLOCKS_PER_SEC);
        printf(",   Time per frame : ");
        Timeprint((double)(end - start) / CLOCKS_PER_SEC);
        printf(",   Remaing time : ");
        Timeprint((double)(numFrames - i) * (double)(end - start) / CLOCKS_PER_SEC);
        start = clock();
        // printf("\n\t\t");
        // print_simple_pendulum(pen);
        // printf("\n");
        // create a blank black image
        // Meas 2
        if (i >= skipFrames)
        {
            startMeasurement(m, 2, i);
            Pixel bg = pixel(0, 0, 0, 255);
            Image img = create_blank_image(imageWidth, imageHeight, 4, bg);
            endMeasurement(m, 2, i);
            if (img == NULL)
            {
                fprintf(stderr, "Failed to create image\n");
                return 1;
            }
            // Meas 3
            startMeasurement(m, 3, i);
            // Measurement mcat3 = create_measurement(numPendulums, 3);
            // for (int j = 0; j < numPendulums; j++)
            // {
            //     if (draw_simple_pendulum(img, drawingPen[j], b, NULL, j) != 0)
            //     {
            //         fprintf(stderr, "Failed to draw pendulum\n");
            //         return 1;
            //     }
            // }

            // MultiThread
            draw_pendulums_mt(img, pens, NULL, numPendulums, b.size, numThreads);
            // printMeasurement(mcat3);
            // free_measurement(mcat3);
            // exit(0);
            endMeasurement(m, 3, i);

            // Meas 4
            startMeasurement(m, 4, i);
            char filename[128];
            sprintf(filename, "resources/circleOfSimplePendulumsMT/images%d.bmp", i);
            // save image
            if (saveImage(img, filename) != 0)
            {
                fprintf(stderr, "Failed to save image\n");
                return 1;
            };
            img = free_image(img);
            endMeasurement(m, 4, i);
        }
        // Meas 5
        startMeasurement(m, 5, i);
        for (int j = 0; j < numPendulums; j++)
        {
            pens[j] = update_simple_pendulum(pens[j], .5);
            if (pens == NULL)
            {
                fprintf(stderr, "Failed to update pendulum\n");
                return 1;
            }
        }
        endMeasurement(m, 5, i);
        // Meas 6
        startMeasurement(m, 6, i);
        for (int j = 0; j < numPendulums; j++)
        {
            // pens[j] = update_simple_color(pens[j]);
            drawingPen[j]->color = pens[j]->color;
        }
        endMeasurement(m, 6, i);
    }
    for (int j = 0; j < numPendulums; j++)
    {
        pens[j] = free_simple_pendulum(pens[j]);
        drawingPen[j] = free_simple_pendulum(drawingPen[j]);
    }
    free(pens);
    free(drawingPen);
    printf("\n");
    return 0;
}

int circleOfDoubleMT(int numFrames, int numPendulums, Measurement m, int skipFrames, int numThreads, double StartAngle, double EndAngle)
{
    int imageWidth = 1000;
    int imageHeight = 1000;

    clock_t start = clock();
    clock_t all = clock();
    clock_t end = clock();

    Brush b = make_circular_brush(7);

    // create a simple pendulum
    // Meas 1
    startMeasurement(m, 1, 0);

    DoublePen *pens = malloc(numPendulums * sizeof(DoublePen));

    if (pens == NULL)
    {
        fprintf(stderr, "Failed to create pendulum\n");
        return 1;
    }
    for (int i = 0; i < numPendulums; i++)
    {

        Pixel color = HSVtoRGB(360 * i / numPendulums, 1, 1);
        color.a = 10;
        double theta = M_PI / 2 + i * (EndAngle - StartAngle)/ numPendulums;
        while (theta > M_PI)
        {
            theta -= 2 * M_PI;
        }
        while (theta < -M_PI)
        {
            theta += 2 * M_PI;
        }
        // printf("i %d : theta %lf\n",i, theta);
        DoublePen pen = create_double_pendulum(imageWidth / 2, imageHeight / 2, theta, 0, 200, 200, 1, color);
        if (pen == NULL)
        {
            fprintf(stderr, "Failed to create pendulum\n");
            return 1;
        }
        
        pens[i] = pen;
    }
    for (int j = 0; j < numPendulums; j++)
    {
        // pens[j] = update_double_color(pens[j], 1);
    }
    endMeasurement(m, 1, 0);
    printf("Starting animation of double pendulum\n");
    for (int i = 0; i < numFrames; i++)
    {
        
        // create a blank black image
        // Meas 2
        if (i >= skipFrames)
        {
            startMeasurement(m, 2, i);
            Pixel bg = pixel(0, 0, 0, 255);
            Image img = create_blank_image(imageWidth, imageHeight, 4, bg);
            endMeasurement(m, 2, i);
            if (img == NULL)
            {
                fprintf(stderr, "Failed to create image\n");
                return 1;
            }
            // Meas 3
            startMeasurement(m, 3, i);
            // Measurement mcat3 = create_measurement(numPendulums, 3);
            // for (int j = 0; j < numPendulums; j++)
            // {
            //     if (draw_simple_pendulum(img, drawingPen[j], b, NULL, j) != 0)
            //     {
            //         fprintf(stderr, "Failed to draw pendulum\n");
            //         return 1;
            //     }
            // }

            // MultiThread
            draw_pendulums_mt(img, NULL, pens, numPendulums, b.size, numThreads);
            // printMeasurement(mcat3);
            // free_measurement(mcat3);
            // exit(0);
            endMeasurement(m, 3, i);

            // Meas 4
            startMeasurement(m, 4, i);
            char filename[128];
            sprintf(filename, "resources/doublePendulumCircle/images%d.bmp", i);
            // save image
            if (saveImage(img, filename) != 0)
            {
                fprintf(stderr, "Failed to save image\n");
                return 1;
            };
            img = free_image(img);
            endMeasurement(m, 4, i);
        }
        // Meas 5
        startMeasurement(m, 5, i);
        for (int j = 0; j < numPendulums; j++)
        {
            pens[j] = update_double_pendulum(pens[j], .05);
            if (pens == NULL)
            {
                fprintf(stderr, "Failed to update pendulum\n");
                return 1;
            }
        }
        endMeasurement(m, 5, i);
        // Meas 6
        startMeasurement(m, 6, i);
        for (int j = 0; j < numPendulums; j++)
        {
           // pens[j] = update_double_color(pens[j], 1);
            
        }
        endMeasurement(m, 6, i);
    }
    for (int j = 0; j < numPendulums; j++)
    {
        pens[j] = free_double_pendulum(pens[j]);
        
    }
    free(pens);
    printf("\n");
    return 0;
}

*/
// int main(void)
// {
//     /*
//     // printf("Start :\n");
//     // printf("\tfirstTest:\n");
//     // if (firstTest() == 0)
//     //     printf("First test passed\n");
//     // printf("\ttest_lines:\n");
//     // if (test_Lines() == 0)
//     //     printf("Lines test passed\n");
//     // printf("Done\n");
    
//     // printf("\tanimation_one_simple_pendulum:\n");
//     // int N = 100;
//     // Measurement m = create_measurement(N, 6);
//     // if (m == NULL)
//     // {
//     //     fprintf(stderr, "Failed to create measurement\n");
//     //     return 1;
//     // }
//     // if (animation_one_simple_pendulum(N, m) == 0){
//     //     printMeasurement(m);
//     //     free_measurement(m);
//     //     printf("Animation test passed\n");
//     // }
//     // printf("Done\n");

//     // printf("\tanimation_Lots_simple_pendulum:\n");
//     // int N2 = 10;
//     // Measurement m2 = create_measurement(N2, 6);
//     // if (m2 == NULL)
//     // {
//     //     fprintf(stderr, "Failed to create measurement\n");
//     //     return 1;
//     // }
//     // if (animation_lots_simple_pendulum(N2, 5000, m2) == 0)
//     // {
//     //     printMeasurement(m2);
//     //     free_measurement(m2);
//     //     printf("Animation test passed\n");
//     // }
//     // printf("Done\n");

//     // printf("\tanimation_Circle of_simple_pendulum:\n");
//     // int N3 = 1000;
//     // Measurement m3 = create_measurement(N3, 6);
//     // if (m3 == NULL)
//     // {
//         //     fprintf(stderr, "Failed to create measurement\n");
//     //     return 1;
//     // }
//     // if (circleOfSimplePendulumsMultiThread(N3, 5000, m3, 0, 4) == 0)
//     // {
//         //     printMeasurement(m3);
//     //     free_measurement(m3);
//     //     printf("Animation test passed\n");
//     // }
//     // printf("Done\n");
    
//     // printf("\tanimation_Circle of double pendulum:\n");
//     // int N3 = 5000;
//     // Measurement m3 = create_measurement(N3, 6);
//     // if (m3 == NULL)
//     // {
//     //     fprintf(stderr, "Failed to create measurement\n");
//     //     return 1;
//     // }
//     // if (circleOfDoubleMT(N3, 5000, m3, 0, 4, 0, 0.1) == 0)
//     // {
//     //     printMeasurement(m3);
//     //     free_measurement(m3);
//     //     printf("Animation test passed\n");
//     // }
//     // printf("Done\n");

//     // printf("\tanimation_Circle of_simple_pendulum:\n");
//     // int N3 = 15;
//     // for (int nt = 1; nt <= 10; nt++){

//     //     Measurement m3 = create_measurement(N3, 6);
//     //     if (m3 == NULL)
//     //     {
//     //         fprintf(stderr, "Failed to create measurement\n");
//     //         return 1;
//     //     }
//     //     if (circleOfSimplePendulumsMultiThread(N3, 5000, m3, 1, nt) == 0)
//     //     {
//         //         printMeasurement(m3);
//     //         printf("    nt = %d\n", nt);
//     //         printf("    time = %lf\nms", m3->durations[2]*1000);
//     //         free_measurement(m3);
//     //         //printf("Animation test passed\n");

//     //     }
//     // }
//     // printf("Done\n");
    
//     */
//     printf("\tanimation_Circle of double pendulum:\n");
    
//     int N_Frames = 3000;
//     Measurement meas = create_measurement(N_Frames, 3);
//     if (animationPendule((GeneralParameters){5000, 1, N_Frames, 0, "test", meas}, (Drawing_Param){2, 2000, 12, 0, M_PI / 2, M_PI / 2 + 0.1, 1, 0.5, .03, 10, "viridis"}) == 0)
//     { // 80 image a dt =1 les image interessante 55 - 58
//         printMeasurement(meas);
//         free_measurement(meas);
//         printf("Animation test passed\n");
//     }
//     printf("Done\n");
//     return 0;
// }

/*
typedef struct {
    int numPendulums;
    int numThreads;
    int numFrames;
    int skipFrames;
    char folder[64];
    Measurement meas;
} GeneralParameters;

typedef struct {
    int typePendulum;
    int sizeWindow;
    int sizeBrush;
    int color_update;
    double start_angle;
    double end_angle;
    int th1or2;
    float PropLengthDouble;
    double dt;
} Drawing_Param;

Here is my main function, but i want to not change the parameter manualy but when i call the exe file fith flags Here is the currunt main file and the diffrent parametters use -flgas=vlaue each time

int main(void)
{int N_Frames = 3000;
    Measurement meas = create_measurement(N_Frames, 3);
    if (animationPendule((GeneralParameters){5000, 1, N_Frames, 0, "test", meas}, (Drawing_Param){2, 2000, 12, 0, M_PI / 2, M_PI / 2 + 0.1, 1, 0.5, .03, 10, "viridis"}) == 0)
    { // 80 image a dt =1 les image interessante 55 - 58
        printMeasurement(meas);
        free_measurement(meas);
        printf("Animation test passed\n");
    }
    printf("Done\n");
    return 0;
}

typedef struct {
    int numPendulums;
    int numThreads;
    int numFrames;
    int skipFrames;
    char folder[64];
    Measurement meas;
} GeneralParameters;

typedef struct {
    int typePendulum;
    int sizeWindow;
    int sizeBrush;  
    int color_update;
    double start_angle;
    double end_angle;
    int th1or2;
    float PropLengthDouble;
    double dt;
    int alpha;
    char cmap[64];
} Drawing_Param;

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for getopt
#include <getopt.h>

int main(int argc, char *argv[])
{
    // Defaults
    GeneralParameters gp = {5000, 1, 3000, 0, "test", NULL};
    Drawing_Param dp = {2, 2000, 12, 0, M_PI / 2, M_PI / 2 + 0.1, 1, 0.5f, 0.03, 10, "viridis"};

    static struct option long_opts[] = {
        {"pendulums", required_argument, 0, 'p'},
        {"threads", required_argument, 0, 'h'},
        {"frames", required_argument, 0, 'f'},
        {"skip", required_argument, 0, 's'},
        {"folder", required_argument, 0, 'o'},

        {"typePendulum", required_argument, 0, 'y'},
        {"window", required_argument, 0, 'w'},
        {"brush", required_argument, 0, 'b'},
        {"color_update", required_argument, 0, 'u'},
        {"start_angle", required_argument, 0, 'i'},
        {"end_angle", required_argument, 0, 'j'},
        {"th1or2", required_argument, 0, 'z'},
        {"propLength", required_argument, 0, 'k'},
        {"dt", required_argument, 0, 't'},
        {"alpha", required_argument, 0, 'a'},
        {"cmap", required_argument, 0, 'c'},
        {0, 0, 0, 0}};

    int opt, idx;
    while ((opt = getopt_long(argc, argv, "p:h:f:s:o:y:w:b:u:i:j:z:k:t:a:c:", long_opts, &idx)) != -1)
    {
        switch (opt)
        {
        case 'p':
            gp.numPendulums = atoi(optarg);
            break;
        case 'h':
            gp.numThreads = atoi(optarg);
            break;
        case 'f':
            gp.numFrames = atoi(optarg);
            break;
        case 's':
            gp.skipFrames = atoi(optarg);
            break;
        case 'o':
            strncpy(gp.folder, optarg, sizeof(gp.folder) - 1);
            break;

        case 'y':
            dp.typePendulum = atoi(optarg);
            break;
        case 'w':
            dp.sizeWindow = atoi(optarg);
            break;
        case 'b':
            dp.sizeBrush = atoi(optarg);
            break;
        case 'u':
            dp.color_update = atoi(optarg);
            break;
        case 'z':
            dp.th1or2 = atoi(optarg);
            break;
        case 'a':
            dp.alpha = atoi(optarg);
            break;
        case 'c':
            strncpy(dp.cmap, optarg, sizeof(dp.cmap) - 1);
            break;

        case 'i':
            dp.start_angle = atof(optarg);
            break;
        case 'j':
            dp.end_angle = atof(optarg);
            break;
        case 'k':
            dp.PropLengthDouble = atof(optarg);
            break;
        case 't':
            dp.dt = atof(optarg);
            break;

        case 0:
            if (strcmp(long_opts[idx].name, "start_angle") == 0)
                dp.start_angle = atof(optarg);
            else if (strcmp(long_opts[idx].name, "end_angle") == 0)
                dp.end_angle = atof(optarg);
            else if (strcmp(long_opts[idx].name, "propLength") == 0)
                dp.PropLengthDouble = atof(optarg);
            else if (strcmp(long_opts[idx].name, "dt") == 0)
                dp.dt = atof(optarg);
            else if (strcmp(long_opts[idx].name, "threads") == 0)
                gp.numThreads = atoi(optarg);
            break;

        default:
            fprintf(stderr, "Usage: %s [options]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // create meas after numFrames is known
    gp.meas = create_measurement(gp.numFrames, 3);

    if (animationPendule(gp, dp) == 0)
    {
        printMeasurement(gp.meas);
        free_measurement(gp.meas);
        printf("Animation test passed\n");
    }
    printf("Done\n");
    return 0;
}