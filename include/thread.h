#ifndef THREAD_H
#define THREAD_H

void start_build_monitor(void);
void stop_build_monitor(void);

void start_build_task(void);
void wait_for_build_task(void);

#endif
