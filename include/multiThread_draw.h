#ifndef _MTDraw_H_
#define _MTDraw_H_

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "line.h"
#include "color.h"
#include "images.h"
#include "pendulum.h"
#include "timing.h"

int draw_pendulums_mt(Image final_img, SimplePen *Spens, DoublePen *Dpens, int numPendulums, int sizeBrush, int numThreads);

#endif