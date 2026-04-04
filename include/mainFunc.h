#ifndef _MainFunc_H_
#define _MainFunc_H_

#include <stdio.h>
#include <stdlib.h>

#include "line.h"
#include "color.h"
#include "images.h"
#include "pendulum.h"
#include "timing.h"
#include "multiThread_draw.h"
#include "draw.h"


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

int animationPendule(GeneralParameters geneP, Drawing_Param drawP);

#endif