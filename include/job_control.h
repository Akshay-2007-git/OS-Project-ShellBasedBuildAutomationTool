
#ifndef JOB_CONTROL_H
#define JOB_CONTROL_H

#include <sys/types.h>

int job_control_init(void);
int job_control_put_foreground(pid_t pgid, int continue_job);
int job_control_put_background(pid_t pgid, int continue_job);

#endif

