#ifndef _TIMING_H_
#define _TIMING_H_


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
// #include <sys/ioctl.h>


typedef struct {
    clock_t start;
    clock_t end;
} Timing;


typedef struct {
    int nbOfMeasurements;
    int nbOfCategories;
    Timing* timings;
    double *durations;
    clock_t start_creation;
} *Measurement;

typedef struct {
    Timing startTime;
    double * framesTimes;
    double meanFrameTime;
    int position;
    Timing timeFrame;
} *Remaining;

#define framesTimesLength 100
#define retrunCarrige 1

Measurement create_measurement(int nbOfMeasurements, int nbOfCategories);
Measurement free_measurement(Measurement measurement);

void startMeasurement(Measurement measurement, int category, int indexOfMeasurement);
void endMeasurement(Measurement measurement, int category, int indexOfMeasurement);

void printMeasurement(Measurement measurement);
void Timeprint(double time);

double duration(clock_t start, clock_t end);

Remaining create_Remaining();
Remaining free_Remaining(Remaining rem);
void update_Remaining(Remaining rem, int i, int numFrames);

#endif