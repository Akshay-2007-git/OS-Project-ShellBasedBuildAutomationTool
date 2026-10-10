
#include "job_control.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

static pid_t shell_pgid = -1;
static int interactive_shell = 0;

int job_control_init(void)
{
    interactive_shell = isatty(STDIN_FILENO);

    if (!interactive_shell) {
        return 0;
    }

    while (tcgetpgrp(STDIN_FILENO) != (shell_pgid = getpgrp())) {
        if (kill(-shell_pgid, SIGTTIN) < 0) {
            perror("kill SIGTTIN");
            return -1;
        }
    }

    if (signal(SIGINT, SIG_IGN) == SIG_ERR ||
        signal(SIGQUIT, SIG_IGN) == SIG_ERR ||
        signal(SIGTSTP, SIG_IGN) == SIG_ERR ||
        signal(SIGTTIN, SIG_IGN) == SIG_ERR ||
        signal(SIGTTOU, SIG_IGN) == SIG_ERR) {
        perror("signal");
        return -1;
    }

    shell_pgid = getpid();

    if (setpgid(shell_pgid, shell_pgid) < 0 &&
        errno != EACCES && errno != EPERM) {
        perror("setpgid shell");
        return -1;
    }

    if (tcsetpgrp(STDIN_FILENO, shell_pgid) < 0) {
        perror("tcsetpgrp shell");
        return -1;
    }

    return 0;
}

int job_control_put_foreground(pid_t pgid, int continue_job)
{
    if (pgid <= 0) {
        return -1;
    }

    if (interactive_shell &&
        tcsetpgrp(STDIN_FILENO, pgid) < 0) {
        perror("tcsetpgrp foreground");
        return -1;
    }

    if (continue_job && kill(-pgid, SIGCONT) < 0) {
        perror("SIGCONT foreground");
        if (interactive_shell) {
            tcsetpgrp(STDIN_FILENO, shell_pgid);
        }
        return -1;
    }

    int status;
    pid_t result;

    do {
        result = waitpid(-pgid, &status, WUNTRACED);
    } while (result < 0 && errno == EINTR);

    if (interactive_shell &&
        tcsetpgrp(STDIN_FILENO, shell_pgid) < 0) {
        perror("tcsetpgrp shell restore");
    }

    if (result < 0) {
        if (errno == ECHILD) {
            return 0;
        }

        perror("waitpid foreground");
        return -1;
    }

    if (WIFSTOPPED(status)) {
        return 1;
    }

    return 0;
}

int job_control_put_background(pid_t pgid, int continue_job)
{
    if (pgid <= 0) {
        return -1;
    }

    if (continue_job && kill(-pgid, SIGCONT) < 0) {
        perror("SIGCONT background");
        return -1;
    }

    return 0;
}
