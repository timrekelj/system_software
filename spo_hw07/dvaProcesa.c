#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

#define MAX_PROCESSES 2

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int sem_id;
int start_time;

void init_semaphore() {
    key_t key = ftok("/tmp", 'S');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = MAX_PROCESSES;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
}

void destroy_semaphore() {
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
}

void wait_semaphore() {
    struct sembuf sem_op = {0, -1, 0};
    if (semop(sem_id, &sem_op, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void signal_semaphore() {
    struct sembuf sem_op = {0, 1, 0};
    if (semop(sem_id, &sem_op, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void *process_function(void *arg) {
    int process_id = *((int *)arg);
    wait_semaphore();
    printf("Process %d begins. (t=%ld)\n", process_id, time(NULL) - start_time);
    sleep(5);
    printf("Process %d ends. (t=%ld)\n", process_id, time(NULL) - start_time);
    signal_semaphore();
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_processes = atoi(argv[1]);
    if (num_processes < 1) {
        fprintf(stderr, "Number of processes should be at least 1.\n");
        exit(EXIT_FAILURE);
    }

    init_semaphore();

    pthread_t *threads = (pthread_t *)malloc(num_processes * sizeof(pthread_t));
    int *process_ids = (int *)malloc(num_processes * sizeof(int));

    start_time = time(NULL);
    for (int i = 0; i < num_processes; i++) {
        process_ids[i] = i;
        pthread_create(&threads[i], NULL, process_function, &process_ids[i]);
    }

    for (int i = 0; i < num_processes; i++) {
        pthread_join(threads[i], NULL);
    }

    destroy_semaphore();

    free(threads);
    free(process_ids);

    return EXIT_SUCCESS;
}

