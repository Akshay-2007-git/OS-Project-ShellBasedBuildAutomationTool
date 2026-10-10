
#include "jobs.h"

#include <stdio.h>
#include <string.h>

static Job job_table[MAX_JOBS];
static int next_job_id = 1;

void jobs_init(void)
{
    memset(job_table, 0, sizeof(job_table));
    next_job_id = 1;
}

int jobs_add(pid_t pgid, const char *command, JobState state)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].id == 0 || job_table[i].state == JOB_DONE) {
            job_table[i].id = next_job_id++;
            job_table[i].pgid = pgid;
            job_table[i].state = state;

            if (command != NULL) {
                snprintf(job_table[i].command,
                         sizeof(job_table[i].command),
                         "%s",
                         command);
            } else {
                job_table[i].command[0] = '\0';
            }

            return job_table[i].id;
        }
    }

    fprintf(stderr, "Job table is full.\n");
    return -1;
}

int jobs_remove(int id)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].id == id) {
            memset(&job_table[i], 0, sizeof(job_table[i]));
            return 0;
        }
    }

    return -1;
}

Job *jobs_find(int id)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].id == id) {
            return &job_table[i];
        }
    }

    return NULL;
}

void jobs_set_state_by_pgid(pid_t pgid, JobState state)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].id != 0 && job_table[i].pgid == pgid) {
            job_table[i].state = state;
        }
    }
}

void jobs_print(void)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (job_table[i].id == 0) {
            continue;
        }

        const char *state_text;

        switch (job_table[i].state) {
            case JOB_RUNNING:
                state_text = "Running";
                break;
            case JOB_STOPPED:
                state_text = "Stopped";
                break;
            case JOB_DONE:
                state_text = "Done";
                break;
            default:
                state_text = "Unknown";
                break;
        }

        printf("[%d] %-8s %s\n",
               job_table[i].id,
               state_text,
               job_table[i].command);
    }
}
