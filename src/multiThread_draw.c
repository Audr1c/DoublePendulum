#include "multiThread_draw.h"
/* ---------- worker pthread ---------- */
// src OVER dst

typedef struct
{
    // input
    Image dst_final;        // pour dimensions uniquement
    SimplePen *Spens;       // tableau Simple Pendulum
    DoublePen *Dpens;       // tableau Simple Pendulum
    int start_idx, end_idx; // [start, end)
    int sizeBrush;          // brush;

    // per-thread buffer (pré-alloué par le main)
    Image temp;
} WorkerArgs;

static void *worker_draw_simple(void *p)
{
    WorkerArgs *w = (WorkerArgs *)p;
    for (int i = w->start_idx; i < w->end_idx; i++)
    {
        draw_simple_pendulum(w->temp, w->Spens[i], w->sizeBrush);
    }
    return NULL;
}
static void *worker_draw_double(void *p)
{
    WorkerArgs *w = (WorkerArgs *)p;
    for (int i = w->start_idx; i < w->end_idx; i++)
    {
        draw_double_pendulum(w->temp, w->Dpens[i], w->sizeBrush);
    }
    return NULL;
}

/* ---------- composition (temp OVER final) ---------- */
static void composite_over(Image dst, Image src)
{
    const size_t N = (size_t)dst->width * (size_t)dst->height;
    for (size_t i = 0; i < N; i++)
        dst->matrix[i] = blend_pixel(dst->matrix[i], src->matrix[i]);
}

/* ---------- API : multi-thread sur N pendules ---------- */
int draw_pendulums_mt(Image final_img, SimplePen *Spens, DoublePen *Dpens, int numPendulums, int sizeBrush, int numThreads)
{
    if (numThreads < 1)
        numThreads = 1;

    pthread_t *ths = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
    WorkerArgs *args = (WorkerArgs *)malloc(sizeof(WorkerArgs) * numThreads);

    // split
    int base = numPendulums / numThreads, rem = numPendulums % numThreads;
    int idx = 0;

    // créer buffers temporaires + lancer
    for (int t = 0; t < numThreads; t++)
    {
        int cnt = base + (t < rem);
        int start = idx, end = idx + cnt;
        idx = end;

        args[t].dst_final = final_img;
        args[t].Spens = Spens;
        args[t].Dpens = Dpens;
        args[t].start_idx = start;
        args[t].end_idx = end;
        args[t].sizeBrush = sizeBrush;
        args[t].temp = create_blank_image(final_img->width, final_img->height, 4, pixel(0, 0, 0, 0)); // img_new(final_img->width, final_img->height);
        if (Dpens != NULL)
        {
            pthread_create(&ths[t], NULL, worker_draw_double, &args[t]);
        }
        else if (Spens != NULL)
        {
            pthread_create(&ths[t], NULL, worker_draw_simple, &args[t]);
        }
        else
        {
            printf("Error : No pendulum type\n");
            return 1;
        }
    }
    
    for (int t = 0; t < numThreads; t++)
    {

        int ret = pthread_join(ths[t], NULL);
        // printf("Hi in here\n");
        composite_over(final_img, args[t].temp);

        free_image(args[t].temp);
    }
    // printf("Hi fin\n");
    free(ths);
    free(args);
    return 0;
}