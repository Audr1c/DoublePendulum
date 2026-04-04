#include "mainFunc.h"


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