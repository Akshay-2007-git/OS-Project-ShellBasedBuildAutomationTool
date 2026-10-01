#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "process.h"

int execute_process(char **tokens)
{
    pid_t pid;
    int status;

    if (tokens == NULL || tokens[0] == NULL)
    {
        return 0;
    }

    /*
     * Create a child process.
     */
    pid = fork();

    if (pid < 0)
    {
        perror("ShellForge: fork");
        return 0;
    }

    /*
     * Child process.
     */
    if (pid == 0)
    {
        /*
         * The parent build tool ignores SIGINT.
         *
         * The child restores the default SIGINT behavior
         * so Ctrl+C can terminate the command being executed.
         */
        if (signal(SIGINT, SIG_DFL) == SIG_ERR)
        {
            perror("ShellForge: signal");
            exit(EXIT_FAILURE);
        }

        /*
         * Replace the child process with the requested
         * program.
         */
        execvp(tokens[0], tokens);

        /*
         * execvp() returns only if an error occurs.
         */
        perror("ShellForge: execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * Parent process.
     *
     * Wait for the foreground child to finish.
     */
    do
    {
        if (waitpid(pid, &status, WUNTRACED) == -1)
        {
            /*
             * If waitpid() was interrupted by a signal,
             * try again.
             */
            if (errno == EINTR)
            {
                continue;
            }

            perror("ShellForge: waitpid");
            return 0;
        }

        break;

    } while (1);

    /*
     * Child completed normally.
     */
    if (WIFEXITED(status))
    {
        printf("Process exited with status: %d\n",
               WEXITSTATUS(status));
    }

    /*
     * Child was terminated by a signal.
     */
    else if (WIFSIGNALED(status))
    {
        printf("Process terminated by signal: %d\n",
               WTERMSIG(status));
    }

    /*
     * Child was stopped.
     */
    else if (WIFSTOPPED(status))
    {
        printf("Process stopped by signal: %d\n",
               WSTOPSIG(status));
    }

    return 1;
}
