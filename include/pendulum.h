#ifndef _PENDULUM_H_
#define _PENDULUM_H_

#include <math.h>
#include "line_openCl.h"
#include "images.h"
#include "line.h"
#include "color.h"
#include "timing.h"


typedef struct
{
    int x, y;   // fixe
    double theta, omega;   // tip
    double length, mass;
    pix color;
} *SimplePen;

typedef struct
{
    int x, y;            // fixe
    double th1, th2, w1, w2; // tip
    double l1, l2, m1,m2;
    pix color;
} *DoublePen;

typedef struct 
{
    SimplePen * spen;
    DoublePen * dpen;
} Pendulum;

#define G 9.81
#define scaleFactor 0.8

SimplePen create_simple_pendulum(int x, int y, double theta, double omega, double length, double mass, pix color);
SimplePen free_simple_pendulum(SimplePen pen);
SimplePen update_simple_pendulum(SimplePen pen, double dt);
int draw_simple_pendulum(Image img, SimplePen pen, int sizeBrush);
// int draw_simple_pendulum(Image img, SimplePen pen, Brush b, Measurement meas, int id);

SimplePen update_simple_color(SimplePen pen);
void print_simple_pendulum(SimplePen pen);

DoublePen create_double_pendulum(int x, int y, double theta1, double theta2, double length1, double length2, double mass, pix color);
DoublePen free_double_pendulum(DoublePen pen);
DoublePen update_double_pendulum(DoublePen pen, double dt);
int draw_double_pendulum(Image img, DoublePen pen, int sizeBrush, Measurement *meas, int i);
int draw_double_pendulum_OCL(OCL_System *ocl, Image img, DoublePen pen, int sizeBrush);

DoublePen update_double_color(DoublePen pen, int theta1or2);

#endif