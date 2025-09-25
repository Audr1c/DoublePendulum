#include "timing.h"
#include <stdio.h>

#define MAXTIME 10

Measurement create_measurement(int nbOfMeasurements, int nbOfCategories){
    Measurement measurement = malloc(sizeof(*measurement));
    if (measurement == NULL){
        fprintf(stderr, "Failed to allocate memory for the measurement pointer\n");
        return NULL;
    }
    measurement->nbOfCategories = nbOfCategories;
    measurement->nbOfMeasurements = nbOfMeasurements;
    measurement->timings = malloc(sizeof(Timing) * nbOfCategories * nbOfMeasurements);
    if (measurement->timings == NULL){
        fprintf(stderr, "Failed to allocate memory for the timings array\n");
        return NULL;
    }
    measurement->durations = malloc(sizeof(double) * nbOfCategories);
    if (measurement->durations == NULL){
        fprintf(stderr, "Failed to allocate memory for the durations array\n");
        return NULL;
    }
    measurement->start_creation = clock();
    
    return measurement;
}
Measurement free_measurement(Measurement measurement){
    free(measurement->timings);
    free(measurement->durations);
    free(measurement);
    return NULL;
}

void startMeasurement(Measurement measurement, int category, int indexOfMeasurement) {
    category--;
    if (category >= measurement->nbOfCategories || indexOfMeasurement >= measurement->nbOfMeasurements)
    {
        fprintf(stderr, "Args too big, category : %d, indexOfMeasurement : %d, nbOfCategories : %d, nbOfMeasurements : %d\n", category, indexOfMeasurement, measurement->nbOfCategories, measurement->nbOfMeasurements);
        exit(4);
    }
    measurement->timings[category * measurement->nbOfMeasurements + indexOfMeasurement].start = clock();
}

void endMeasurement(Measurement measurement, int category, int indexOfMeasurement) {
    category--;
    if (category >= measurement->nbOfCategories || indexOfMeasurement >= measurement->nbOfMeasurements){
        fprintf(stderr, "Args too big, category : %d, indexOfMeasurement : %d, nbOfCategories : %d, nbOfMeasurements : %d\n", category, indexOfMeasurement, measurement->nbOfCategories, measurement->nbOfMeasurements);
        exit(4);
    }
    measurement->timings[category * measurement->nbOfMeasurements + indexOfMeasurement].end = clock();
}

double duration(clock_t start, clock_t end) {
    return (double)(end - start) / CLOCKS_PER_SEC;
}
static inline double durationTim(Timing timing) {
    return duration(timing.start, timing.end);
}
void meandurations(Measurement measurement) {
    
    for (int i = 0; i < measurement->nbOfCategories; i++) {
        measurement->durations[i] = 0;
        for (int j = 0; j < measurement->nbOfMeasurements; j++) {
            measurement->durations[i] += durationTim(measurement->timings[i * measurement->nbOfMeasurements + j]);
        }
        measurement->durations[i] /= measurement->nbOfMeasurements;
        if (measurement->durations[i] < 0 || measurement->durations[i] > MAXTIME)
        {
            measurement->durations[i] = 0;
        }
    }    
}
void printMeasurement(Measurement measurement) {
    meandurations(measurement);
    double sum = 0;
    for (int i = 0; i < measurement->nbOfCategories; i++) {
        sum += measurement->durations[i];
    }
    printf("Total time: %fms\n", sum*1000);
    for (int i = 0; i < measurement->nbOfCategories; i++) {
        printf("\tCategory %d: %fms, (%.1f%%)\n", i + 1, measurement->durations[i] * 1000, measurement->durations[i] / sum * 100);
    }
    printf("Time just for measurment = %fms\n", duration(measurement->start_creation, clock()) * 1000 - sum * 1000);
    printf("End Meas\n");
}

void Timeprint(double time) {
    if (time < 0)
        return;
    // print the time in hh:mm:ss format
    int h = time / 3600;
    int m = (time - h * 3600) / 60;
    int s = time - h * 3600 - m * 60;
    int ms = (time - h * 3600 - m * 60 - s) * 1000;
    if (h == 0){
        if (m == 0){
            if (s == 0)
                printf("%dms", ms);
            else
                printf("%ds,%03dms", s, ms);
        }
        else 
            printf("%dm%02ds,%03dms", m, s, ms);
    }
    else 
        printf("%dh%02dm%02ds,%03dms", h, m, s, ms);
}


Remaining create_Remaining(){

    Remaining rem = malloc(sizeof(*rem));
    if (rem == NULL){
        fprintf(stderr, "Failed to allocate memory for the remaining pointer\n");
        return NULL;
    }
    rem->framesTimes = calloc(sizeof(double), framesTimesLength);
    if (rem->framesTimes == NULL){
        fprintf(stderr, "Failed to allocate memory for the framesTimes array\n");
        free(rem); // Prevent memory leak
        return NULL;
    }
    rem->startTime.start = clock();
    rem->position = 0;
    return rem;
}

Remaining free_Remaining(Remaining rem){
    free(rem->framesTimes);
    free(rem);
    return NULL;
}

inline static void calculate_frameTime(Remaining rem){
    rem->meanFrameTime = 0;
    int count = 0;
    for(int i = 0; i < framesTimesLength; i++){
        if (rem->framesTimes[i] <= 0){
            continue;
        }
        count++;
        rem->meanFrameTime += rem->framesTimes[i];
    }
    rem->meanFrameTime /= count;
}

void update_Remaining(Remaining rem, int frame, int numFrames){
    rem->timeFrame.end = clock();
    rem->startTime.end = clock();

    // update frame time
    rem->framesTimes[rem->position] = durationTim(rem->timeFrame);
    rem->position = (rem->position + 1) % framesTimesLength;


    


    calculate_frameTime(rem);
    if (retrunCarrige == 1)
        printf("\r");
    else
        printf("\n");

    printf("Running: frame %d (%.2f%%),", frame, frame / (float)numFrames * 100);
    printf("    T Elapsed : ");
    Timeprint(durationTim(rem->startTime));
    printf(",   SPF: ");
    Timeprint(rem->meanFrameTime);
    printf(",   Remaing : ");
    Timeprint((numFrames - frame) * rem->meanFrameTime);

    rem->timeFrame.start = clock();
}