#include "pendulum.h"

/* pendulums.c
   Mise à jour d'un simple et d'un double pendule.
   - simple: semi-implicit Euler (stable, très rapide)
   - double: RK4 sur le système d'ordre 1 (theta,omega)
   Note: pas d'approximation petits angles (utilise sin/cos).
*/


/* ---- Simple pendulum ----
   theta: pointeur angle (rad)
   omega: pointeur vitesse angulaire (rad/s)
   dt   : pas (s)
   g    : gravité (ex: 9.81)
   L    : longueur (m)
   m    : masse (kg) (utilisé si amortissement choisi)
   b    : amortissement visqueux sur le moment (N·m·s) ; mettre 0 si pas d'amortissement
*/
SimplePen create_simple_pendulum(int x, int y, double theta, double omega, double length, double mass, pix color){
    SimplePen pen = malloc(sizeof(*pen));
    if(pen == NULL){
        fprintf(stderr, "Failed to allocate memory for the pendulum pointer\n");
        return NULL;
    }
    while (theta > M_PI)
    {
        theta -= 2 * M_PI;
    }
    while (theta < -M_PI)
    {
        theta += 2 * M_PI;
    }
    pen->x = x;
    pen->y = y;
    pen->theta = theta;
    pen->omega = omega;
    pen->length = length;
    pen->mass = mass;
    pen->color = color;
    return pen;
}
SimplePen free_simple_pendulum(SimplePen pen){
    free(pen);
    return NULL;
}

//int draw_simple_pendulum(Image img, SimplePen pen, Brush b, Measurement meas, int id){
int draw_simple_pendulum(Image img, SimplePen pen, int sizeBrush)
{
    // convert theta to x1,y1, x2,y2
    // startMeasurement(meas, 1, id);
    if (pen == NULL){
        fprintf(stderr, "pen is NULL\n");
        return 1;
    }
    int x1 = pen->x;
    int y1 = pen->y;
    if (x1 < 0 || y1 < 0 || x1 > img->width || y1 > img->height){
        fprintf(stderr, "pendulum out of bounds\n");
        print_simple_pendulum(pen);
        return 1;
    }
    if (pen->theta > 2 * M_PI || pen->theta < - 2* M_PI){
        fprintf(stderr, "pendulum theta out of bounds %lf\n", pen->theta);
        print_simple_pendulum(pen);
        return 1;
    }
    // endMeasurement(meas, 1, id);
    // startMeasurement(meas, 2, id);
    double scale = img->height / 100 / 2;
    int x2 = x1 + pen->length * scale * scaleFactor * cos(pen->theta+M_PI/2);
    int y2 = y1 + pen->length * scale * scaleFactor * sin(pen->theta + M_PI / 2);
    // endMeasurement(meas, 2, id);
    // startMeasurement(meas, 3, id);
    //draw_line_thick_smooth(img, x1, y1, x2, y2, b, pen->color);
    // printf("I am the pendulum : %lf, %lf\n", pen->theta, pen->omega);
    draw_thick_line_fast(img, x1, y1, x2, y2, sizeBrush, pen->color);
    // printf("I am done the pendulum : %lf, %lf\n", pen->theta, pen->omega);
    // endMeasurement(meas, 3, id);
    return 0;
}

SimplePen update_simple_pendulum(SimplePen pen, double dt)
{
    /* acceleration α = - (g/L) * sin(theta) + damping_term */
    double th = pen->theta;
    double w = pen->omega;
    double alpha = -(G / pen->length) * sin(th);
    // printf("Calcul: \n");
    // printf("w %lf\n", w);
    // printf("th %lf\n", th);
     // if (b != 0.0)
    //     alpha += -(b / (pen->mass * pen->length * pen->length)) * w; /* transforme couple en alpha */
    //     alpha += -(b / (m * L * L)) * w; /* transforme couple en alpha */
    /* semi-implicit Euler */
    w += dt * alpha;
    th += dt * w;
    
    pen->theta = th;
    
    while (th > M_PI){
        th -= 2 * M_PI;
    }
    while (th < -M_PI){
        th += 2 * M_PI;
    }
    
    pen->omega = w;
    
    return pen;
}

void print_simple_pendulum(SimplePen pen){
    printf("theta: %lf, omega: %lf", pen->theta, pen->omega);
    printf("\t x1: %d, y1: %d", pen->x, pen->y);
}

SimplePen update_simple_color(SimplePen pen){
    // the color is a HSV color that the hue is the theta of the pendulum
    double th = pen->theta;
    double s = 1.0;
    double v = 1.0;
    int a = pen->color.a;
    // printf("theta: %lf\n", th * 180 / M_PI);
    pen->color = rgbTopix(HSVtoRGB(th * 180 / M_PI, s, v), a);
    // printf("color: %d %d %d\n", pen->color.r, pen->color.g, pen->color.b);
    // pen->color.a = a;
    return pen;
}

/* ---- Double pendulum (general m1,m2,L1,L2) ----
   theta1, omega1, theta2, omega2 : pointeurs (rad, rad/s)
   dt : pas
   m1, m2 : masses
   L1, L2 : longueurs
   g : gravité
   b1, b2 : amortissements visqueux appliqués aux vitesses angulaires (couple -> alpha)
*/

DoublePen create_double_pendulum(int x, int y, double theta1, double theta2, double length1, double length2, double mass, pix color)
{
    DoublePen pen = malloc(sizeof(*pen));
    if(pen == NULL){
        fprintf(stderr, "Failed to allocate memory for the pendulum pointer\n");
        return NULL;
    }
    while (theta1 > M_PI)
    {
        theta1 -= 2 * M_PI;
    }
    while (theta1 < -M_PI)
    {
        theta1 += 2 * M_PI;
    }
    while (theta2 > M_PI)
    {
        theta2 -= 2 * M_PI;
    }
    while (theta2 < -M_PI)
    {
        theta2 += 2 * M_PI;
    }
    pen->x = x;
    pen->y = y;
    pen->th1 = theta1;
    pen->w1 = 0;
    pen->th2 = theta2;
    pen->w2 = 0;
    pen->l1 = length1;
    pen->l2 = length2;
    pen->m1 = mass;
    pen->m2 = mass;
    pen->color = color;
    return pen;
}
DoublePen free_double_pendulum(DoublePen pen){
    free(pen);
    return NULL;
}

int draw_double_pendulum(Image img, DoublePen pen, int sizeBrush)
{
    // convert theta to x1,y1, x2,y2, x3,y3
    int x1 = pen->x;
    int y1 = pen->y;
    double scale = img->height / 100 / 2;
    int x2 = x1 + pen->l1 * scale * scaleFactor * cos(pen->th1 + M_PI / 2);
    int y2 = y1 + pen->l1 * scale * scaleFactor * sin(pen->th1 + M_PI / 2);

    int x3 = x2 + pen->l2 * scale * scaleFactor * cos(pen->th2 - M_PI / 2);
    int y3 = y2 + pen->l2 * scale * scaleFactor * sin(pen->th2 - M_PI / 2);

    draw_thick_line_fast(img, x2, y2, x3, y3, sizeBrush, pen->color);
    draw_thick_line_fast(img, x1, y1, x2, y2, sizeBrush, pen->color);
    return 0;
}

static inline void compute_double_acc(const double th1, const double w1,
                                      const double th2, const double w2,
                                      double m1, double m2, double L1, double L2,
                                      double *a1_out, double *a2_out)
{
    double delta = th2 - th1;
    double sDelta = sin(delta);
    double cDelta = cos(delta);
    double s1 = sin(th1);
    double s2 = sin(th2);
    double w1sq = w1 * w1;
    double w2sq = w2 * w2;

    /* Denominator used in standard closed-form */
    double denom = 2.0 * m1 + m2 - m2 * cos(2.0 * delta);
    /* avoid divide by zero: if denom very small, perturb slightly (rare) */
    if (fabs(denom) < 1e-12)
        denom = (denom < 0.0) ? -1e-12 : 1e-12;

    /* acceleration formulas (standard form) */
    double num1 = -G * (2.0 * m1 + m2) * s1 - m2 * G * sin(th1 - 2.0 * th2) - 2.0 * sDelta * m2 * (w2sq * L2 + w1sq * L1 * cDelta);
    double a1 = num1 / (L1 * denom);

    double num2 = 2.0 * sDelta * (w1sq * L1 * (m1 + m2) + G * (m1 + m2) * cos(th1) + w2sq * L2 * m2 * cDelta);
    double a2 = num2 / (L2 * denom);

    /* add simple viscous damping (as torque -> alpha) */
    // if (b1 != 0.0)
    //     a1 += -(b1 / (m1 * L1 * L1)) * w1;
    // if (b2 != 0.0)
    //     a2 += -(b2 / (m2 * L2 * L2)) * w2;

    *a1_out = a1;
    *a2_out = a2;
}

DoublePen update_double_pendulum(DoublePen pen, double dt){
    double th1 = pen -> th1;
    double w1 = pen->w1;
    double th2 = pen->th2;
    double w2 = pen->w2;
    double L1 = pen->l1;
    double L2 = pen->l2;
    double m1 = pen->m1;
    double m2 = pen->m2;
    /* RK4 on the 1st-order system:
    y = [th1, w1, th2, w2]
    y' = [w1, a1(y), w2, a2(y)]
    */
    double k1_th1, k1_w1, k1_th2, k1_w2;
    double k2_th1, k2_w1, k2_th2, k2_w2;
    double k3_th1, k3_w1, k3_th2, k3_w2;
    double k4_th1, k4_w1, k4_th2, k4_w2;

    double a1, a2;

    /* k1 */
    compute_double_acc(th1, w1, th2, w2, m1, m2, L1, L2, &a1, &a2);
    k1_th1 = w1;
    k1_w1 = a1;
    k1_th2 = w2;
    k1_w2 = a2;

    /* k2 */
    {
        double th1_ = th1 + 0.5 * dt * k1_th1;
        double w1_ = w1 + 0.5 * dt * k1_w1;
        double th2_ = th2 + 0.5 * dt * k1_th2;
        double w2_ = w2 + 0.5 * dt * k1_w2;
        compute_double_acc(th1_, w1_, th2_, w2_, m1, m2, L1, L2, &a1, &a2);
        k2_th1 = w1_;
        k2_w1 = a1;
        k2_th2 = w2_;
        k2_w2 = a2;
    }

    /* k3 */
    {
        double th1_ = th1 + 0.5 * dt * k2_th1;
        double w1_ = w1 + 0.5 * dt * k2_w1;
        double th2_ = th2 + 0.5 * dt * k2_th2;
        double w2_ = w2 + 0.5 * dt * k2_w2;
        compute_double_acc(th1_, w1_, th2_, w2_, m1, m2, L1, L2, &a1, &a2);
        k3_th1 = w1_;
        k3_w1 = a1;
        k3_th2 = w2_;
        k3_w2 = a2;
    }

    /* k4 */
    {
        double th1_ = th1 + dt * k3_th1;
        double w1_ = w1 + dt * k3_w1;
        double th2_ = th2 + dt * k3_th2;
        double w2_ = w2 + dt * k3_w2;
        compute_double_acc(th1_, w1_, th2_, w2_, m1, m2, L1, L2, &a1, &a2);
        k4_th1 = w1_;
        k4_w1 = a1;
        k4_th2 = w2_;
        k4_w2 = a2;
    }

    /* combine */
    pen->th1 += dt * (k1_th1 + 2.0 * k2_th1 + 2.0 * k3_th1 + k4_th1) / 6.0;
    pen->w1 += dt * (k1_w1 + 2.0 * k2_w1 + 2.0 * k3_w1 + k4_w1) / 6.0;
    pen->th2 += dt * (k1_th2 + 2.0 * k2_th2 + 2.0 * k3_th2 + k4_th2) / 6.0;
    pen->w2 += dt * (k1_w2 + 2.0 * k2_w2 + 2.0 * k3_w2 + k4_w2) / 6.0;

    return pen;
}

DoublePen update_double_color(DoublePen pen, int theat1or2){
    // the color is a HSV color that the hue is the theta of the pendulum
    double th;
    if (theat1or2 == 1)
        th = pen->th1;
    else
        th = pen->th2;
    double s = 1.0;
    double v = 1.0;
    int a = pen->color.a;
    pen->color = rgbTopix(HSVtoRGB(th * 180 / M_PI, s, v), a);
    // pen->color = HSVtoRGB(th*180/M_PI, s, v);
    return pen;
}
