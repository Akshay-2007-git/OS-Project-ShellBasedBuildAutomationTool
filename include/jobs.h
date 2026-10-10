
#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

#define MAX_JOBS 128
#define JOB_COMMAND_LENGTH 256

typedef enum {
    JOB_RUNNING,
    JOB_STOPPED,
    JOB_DONE
} JobState;

typedef struct {
    int id;
    pid_t pgid;
    JobState state;
    char command[JOB_COMMAND_LENGTH];
} Job;

void jobs_init(void);
int jobs_add(pid_t pgid, const char *command, JobState state);
int jobs_remove(int id);
Job *jobs_find(int id);
void jobs_set_state_by_pgid(pid_t pgid, JobState state);
void jobs_print(void);

#endif
