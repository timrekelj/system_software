#include <bits/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define Ps_MAX 1e9

float sqrt2 = 0.0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int thread_count = 0;

int Ps = 0;
int Zs = 0;

void* monte_carlo(void* arg) {
    struct timespec time;
    int p = 0;
    int z = 0;

    double x;

    while (p < Ps_MAX / thread_count) {
        clock_gettime(CLOCK_REALTIME, &time);
        x = (double)rand_r((unsigned int*)&time.tv_nsec) / RAND_MAX * 2.0;
        z += (x * x < 2.0);
        p += 1;
    }

    pthread_mutex_lock(&mutex);
    Ps += p;
    Zs += z;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (argc != 2) {
        printf("Usage: %s <number of threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    thread_count = atoi(argv[1]);
    if (thread_count < 1) {
        printf("Number of threads should be at least 1.\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[thread_count];
    int thread_indices[thread_count];
    
    srand(time(NULL));

    for (int i = 0; i < thread_count; i++) {
        thread_indices[i] = i;
        pthread_create(&threads[i], NULL, monte_carlo, &thread_indices[i]);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    double elapsed_time = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) * 1e-9;
    printf("Finished in %.3f seconds.\n", elapsed_time);
    printf("Approximation of sqrt(2): %.10f.\n", 2.0 * ((double)Zs/Ps));

    return EXIT_SUCCESS;
}
