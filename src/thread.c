#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "thread.h"

static pthread_t monitor_thread;
static pthread_t build_thread;

static pthread_mutex_t build_lock = PTHREAD_MUTEX_INITIALIZER;

static int build_running = 0;
static int build_completed = 0;

static void *monitor_function(void *arg)
{
    (void)arg;

    while (1)
    {
        pthread_mutex_lock(&build_lock);

        if (build_completed)
        {
            pthread_mutex_unlock(&build_lock);
            break;
        }

        if (build_running)
        {
            printf("\n[Monitor] Build is running...\n");
            fflush(stdout);
        }

        pthread_mutex_unlock(&build_lock);

        sleep(2);
    }

    return NULL;
}

static void *build_function(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&build_lock);
    build_running = 1;
    build_completed = 0;
    pthread_mutex_unlock(&build_lock);

    printf("\n[Build] Build task started.\n");
    fflush(stdout);

    sleep(3);

    printf("[Build] Compiling project files...\n");
    fflush(stdout);

    sleep(3);

    printf("[Build] Linking project...\n");
    fflush(stdout);

    sleep(2);

    pthread_mutex_lock(&build_lock);

    build_running = 0;
    build_completed = 1;

    pthread_mutex_unlock(&build_lock);

    printf("[Build] Build task completed successfully.\n");
    fflush(stdout);

    return NULL;
}

void start_build_monitor(void)
{
    int result;

    pthread_mutex_lock(&build_lock);
    build_running = 0;
    build_completed = 0;
    pthread_mutex_unlock(&build_lock);

    result = pthread_create(
        &monitor_thread,
        NULL,
        monitor_function,
        NULL
    );

    if (result != 0)
    {
        fprintf(stderr, "[Thread] Failed to create monitor thread.\n");
    }
}

void stop_build_monitor(void)
{
    pthread_mutex_lock(&build_lock);
    build_completed = 1;
    pthread_mutex_unlock(&build_lock);

    pthread_join(monitor_thread, NULL);
}

void start_build_task(void)
{
    int result;

    result = pthread_create(
        &build_thread,
        NULL,
        build_function,
        NULL
    );

    if (result != 0)
    {
        fprintf(stderr, "[Thread] Failed to create build thread.\n");
        return;
    }
}

void wait_for_build_task(void)
{
    pthread_join(build_thread, NULL);
}
