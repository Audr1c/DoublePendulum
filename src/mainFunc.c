#include "mainFunc.h"

int animationPendule(GeneralParameters geneP, Drawing_Param drawP)
{
    int anchors_len;
    Anchor *anchors = load_colormap("Cmap/colorsmaps.json", drawP.cmap, &anchors_len);
    if (!anchors)
    {
        printf("Error loading colormap\n");
        return 1;
    }
    

    Pendulum pend;
    pend.dpen = NULL;
    pend.spen = NULL;
    // create the pendulums
    if (drawP.typePendulum == 1){
        SimplePen *pens = malloc(geneP.numPendulums * sizeof(SimplePen));
        
        pend.spen = pens;
    }
    else if (drawP.typePendulum == 2)
    {
        DoublePen *pens = malloc(geneP.numPendulums * sizeof(SimplePen));
        pend.dpen = pens;
    }
    else
        return 1;
    
    if (pend.dpen == NULL && pend.spen == NULL){
        fprintf(stderr, "Failed to create pendulum\n");
        return 1;
    }

    for (int i = 0; i < geneP.numPendulums; i++){
        // Pixel color = HSVtoRGB(360 * i / geneP.numPendulums, 1, 1);
        Pixel color = rgbTopix(getColor((double)i / geneP.numPendulums, anchors, anchors_len), drawP.alpha);
        
        // printf(" i: %d ,Color = %d %d %d %d\n", i,color.r, color.g, color.b, color.a);
        double theta = (double)i / geneP.numPendulums * (drawP.end_angle - drawP.start_angle) + drawP.start_angle;
        if (drawP.typePendulum == 1){
            pend.spen[i] = create_simple_pendulum(drawP.sizeWindow / 2, drawP.sizeWindow / 2, theta, 0, 100, 1, color);
        }
        if (drawP.typePendulum == 2)
        {
            double theta1 = M_PI;
            double theta2 = 0;
            if (drawP.th1or2)
                theta1 = theta;
            else
                theta2 = theta;
            pend.dpen[i] = create_double_pendulum(drawP.sizeWindow / 2, drawP.sizeWindow / 2, theta1, theta2, 100 * drawP.PropLengthDouble, 100 *(1-drawP.PropLengthDouble), 1, color);
        }
        
    }

    Remaining rem = create_Remaining();
    printf("Starting animation of Pendulum\n");
    // double *d = malloc(sizeof(*d));
    // printf("HI %d\n", d);
    if (rem == NULL){
        fprintf(stderr, "Failed to create remaining\n");
        return 1;
    }
    for (int frame = 0; frame < geneP.numFrames; frame++){
        update_Remaining(rem, frame - geneP.skipFrames, geneP.numFrames-geneP.skipFrames);
        if (geneP.skipFrames <= frame){
            Pixel bg = pixel(0, 0, 0, 255);
            Image img = create_blank_image(drawP.sizeWindow, drawP.sizeWindow, 4, bg);
            if (img == NULL)
            {
                fprintf(stderr, "Failed to create image\n");
                return 1;
            }
            // draw the pendulums
            if (geneP.meas != NULL)
                startMeasurement(geneP.meas, 1, frame);
            // if (frame == 5)
                // printf("HI\n");
            
            if (drawP.typePendulum == 1){
                if (draw_pendulums_mt(img, pend.spen, NULL, geneP.numPendulums, drawP.sizeBrush, geneP.numThreads) != 0)
                {
                    fprintf(stderr, "Failed to draw pendulums\n");
                    return 1;
                };
            }
            else if (drawP.typePendulum == 2)
            {
                // if (draw_pendulums_mt(img, NULL, pend.dpen, geneP.numPendulums, drawP.sizeBrush, geneP.numThreads) != 0)
                // {
                //     fprintf(stderr, "Failed to draw pendulums\n");
                //     return 1;
                // };
                if (draw_pendulums(img, pend.dpen, geneP.numPendulums, drawP.sizeBrush) != 0)
                {
                    fprintf(stderr, "Failed to draw pendulums\n");
                    return 1;
                };

            }
            if (geneP.meas != NULL)
                endMeasurement(geneP.meas, 1, frame);
            
            // create image
            if (geneP.meas != NULL)
                startMeasurement(geneP.meas, 2, frame);
            
            if (createImage(img, geneP.folder, frame) != 0)
            {
                fprintf(stderr, "Failed to save Image\n");
                return 1;
            };
            if (geneP.meas != NULL)
                endMeasurement(geneP.meas, 2, frame);
        }
        // update the pendulums
        if (geneP.meas != NULL)
        startMeasurement(geneP.meas, 3, frame);
        
        for (int pen = 0; pen < geneP.numPendulums; pen++){
            double dt = drawP.dt;
            int tour = 1;
            if (dt > 0.1){
                tour = (int)dt / 0.1 + 1;
                dt = dt/tour;
            }
            for (int t = 0; t < tour; t++){
                
                if (drawP.typePendulum == 1){
                    pend.spen[pen] = update_simple_pendulum(pend.spen[pen], dt);
                    if (pend.spen[pen] == NULL)
                    {
                        fprintf(stderr, "Failed to draw pendulums\n");
                        return 1;
                    }
                    if (drawP.color_update == 1){
                        pend.spen[pen] = update_simple_color(pend.spen[pen]);
                    }
                }
                else if (drawP.typePendulum == 2){
                    pend.dpen[pen]  = update_double_pendulum(pend.dpen[pen], dt);
                    
                    if (pend.dpen[pen] == NULL)
                    {
                        fprintf(stderr, "Failed to draw pendulums\n");
                        return 1;
                    }
                    if (drawP.color_update == 1){
                        pend.dpen[pen] = update_double_color(pend.dpen[pen], drawP.th1or2);
                    }
                }
            }
        }
        if (geneP.meas != NULL)
            endMeasurement(geneP.meas, 3, frame);
        
    }
    // free all allocted memory 
    for (int i = 0; i < geneP.numPendulums; i++){
        if (pend.spen != NULL)
            pend.spen[i] = free_simple_pendulum(pend.spen[i]);
        if (pend.dpen != NULL)
            pend.dpen[i] = free_double_pendulum(pend.dpen[i]);
    }
    
    rem = free_Remaining(rem);
    free(anchors);
    printf("Done\n");
    return 0;
}