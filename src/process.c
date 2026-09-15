#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    pid = fork();

    if (pid < 0)
    {
        perror("ShellForge: fork");
        return 0;
    }

    if (pid == 0)
    {
        /*
         * Child process
         *
         * Replace the child process with the requested program.
         */
        execvp(tokens[0], tokens);

        /*
         * execvp() returns only if an error occurs.
         */
        perror("ShellForge: execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * Parent process waits for the child.
     */
    do
    {
        if (waitpid(pid, &status, WUNTRACED) == -1)
        {
            perror("ShellForge: waitpid");
            return 0;
        }

    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    if (WIFEXITED(status))
    {
        printf("Process exited with status: %d\n",
               WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("Process terminated by signal: %d\n",
               WTERMSIG(status));
    }

    return 1;
}
