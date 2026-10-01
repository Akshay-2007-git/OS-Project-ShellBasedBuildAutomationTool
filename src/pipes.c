#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pipes.h"

int execute_pipe(char **cmd1, char **cmd2)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int status1;
    int status2;

    if (cmd1 == NULL || cmd1[0] == NULL ||
        cmd2 == NULL || cmd2[0] == NULL)
    {
        printf("Invalid pipe command\n");
        return 0;
    }

    /*
     * Create the pipe.
     *
     * pipefd[0] = read end
     * pipefd[1] = write end
     */
    if (pipe(pipefd) == -1)
    {
        perror("ShellForge: pipe");
        return 0;
    }

    /*
     * Create the first child.
     */
    pid1 = fork();

    if (pid1 == -1)
    {
        perror("ShellForge: fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return 0;
    }

    if (pid1 == 0)
    {
        /*
         * The first child writes into the pipe,
         * so it does not need the read end.
         */
        close(pipefd[0]);

        /*
         * Restore normal Ctrl+C behavior for the child.
         */
        if (signal(SIGINT, SIG_DFL) == SIG_ERR)
        {
            perror("ShellForge: signal");
            exit(EXIT_FAILURE);
        }

        /*
         * Redirect stdout to the pipe.
         */
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("ShellForge: dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);

        /*
         * Execute the first command.
         */
        execvp(cmd1[0], cmd1);

        perror("ShellForge: execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * Create the second child.
     */
    pid2 = fork();

    if (pid2 == -1)
    {
        perror("ShellForge: fork");

        close(pipefd[0]);
        close(pipefd[1]);

        /*
         * Wait for the first child so it does not
         * become a zombie.
         */
        waitpid(pid1, NULL, 0);

        return 0;
    }

    if (pid2 == 0)
    {
        /*
         * The second child reads from the pipe,
         * so it does not need the write end.
         */
        close(pipefd[1]);

        /*
         * Restore normal Ctrl+C behavior for the child.
         */
        if (signal(SIGINT, SIG_DFL) == SIG_ERR)
        {
            perror("ShellForge: signal");
            exit(EXIT_FAILURE);
        }

        /*
         * Redirect stdin from the pipe.
         */
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("ShellForge: dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);

        /*
         * Execute the second command.
         */
        execvp(cmd2[0], cmd2);

        perror("ShellForge: execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * Parent does not use either end of the pipe.
     */
    close(pipefd[0]);
    close(pipefd[1]);

    /*
     * Wait for both children.
     */
    do
    {
        if (waitpid(pid1, &status1, 0) == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("ShellForge: waitpid");
            break;
        }

        break;

    } while (1);

    do
    {
        if (waitpid(pid2, &status2, 0) == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("ShellForge: waitpid");
            break;
        }

        break;

    } while (1);

    /*
     * Display the exit status of both commands.
     */
    if (WIFEXITED(status1))
    {
        printf("First command exited with status: %d\n",
               WEXITSTATUS(status1));
    }
    else if (WIFSIGNALED(status1))
    {
        printf("First command terminated by signal: %d\n",
               WTERMSIG(status1));
    }

    if (WIFEXITED(status2))
    {
        printf("Second command exited with status: %d\n",
               WEXITSTATUS(status2));
    }
    else if (WIFSIGNALED(status2))
    {
        printf("Second command terminated by signal: %d\n",
               WTERMSIG(status2));
    }

    return 1;
}
