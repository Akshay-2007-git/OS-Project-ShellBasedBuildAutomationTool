#include <stdio.h>
#include <signal.h>

#include "signals.h"

static void handle_sigint(int signal_number)
{
    (void)signal_number;

    printf("\n");
    printf("Build tool is still running. Type 'exit' to quit.\n");
    printf("build> ");
    fflush(stdout);
}

static void handle_sigchld(int signal_number)
{
    (void)signal_number;
}

void initialize_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_chld;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        perror("ShellForge: sigaction(SIGINT)");
    }

    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1)
    {
        perror("ShellForge: sigaction(SIGCHLD)");
    }
}
