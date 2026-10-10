
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

static void *worker_one(void *arg)
{
    (void)arg;

    printf("Worker 1: waiting for lock A\n");
    pthread_mutex_lock(&lock_a);
    printf("Worker 1: acquired lock A\n");

    sleep(1);

    printf("Worker 1: waiting for lock B\n");
    pthread_mutex_lock(&lock_b);

    printf("Worker 1: acquired lock B\n");

    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);

    return NULL;
}

static void *worker_two(void *arg)
{
    (void)arg;

    printf("Worker 2: waiting for lock B\n");
    pthread_mutex_lock(&lock_b);
    printf("Worker 2: acquired lock B\n");

    sleep(1);

    printf("Worker 2: waiting for lock A\n");
    pthread_mutex_lock(&lock_a);

    printf("Worker 2: acquired lock A\n");

    pthread_mutex_unlock(&lock_a);
    pthread_mutex_unlock(&lock_b);

    return NULL;
}

int main(void)
{
    pthread_t thread_one;
    pthread_t thread_two;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (pthread_create(&thread_one, NULL, worker_one, NULL) != 0) {
        perror("pthread_create worker_one");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread_two, NULL, worker_two, NULL) != 0) {
        perror("pthread_create worker_two");
        return EXIT_FAILURE;
    }

    printf("Deadlock demonstration started.\n");
    printf("If both workers hold one lock and wait for the other, the program hangs.\n");

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    pthread_mutex_destroy(&lock_a);
    pthread_mutex_destroy(&lock_b);

    printf("Both workers completed.\n");
    return EXIT_SUCCESS;
}
