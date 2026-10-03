#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "redirect.h"

static int wait_for_child(pid_t pid)
{
    int status;

    do
    {
        if (waitpid(pid, &status, 0) == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("ShellForge: waitpid");
            return 0;
        }

        break;

    } while (1);

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

int execute_redirection(char **args)
{
    int i;

    if (args == NULL || args[0] == NULL)
    {
        return 0;
    }

    for (i = 0; args[i] != NULL; i++)
    {
        /*
         * Output redirection: >
         *
         * Example:
         * ls > files.txt
         */
        if (strcmp(args[i], ">") == 0)
        {
            int fd;
            pid_t pid;

            if (args[i + 1] == NULL)
            {
                fprintf(stderr,
                        "ShellForge: missing file after '>'\n");
                return 1;
            }

            args[i] = NULL;

            fd = open(args[i + 1],
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

            if (fd < 0)
            {
                perror("ShellForge: open");
                return 1;
            }

            pid = fork();

            if (pid < 0)
            {
                perror("ShellForge: fork");
                close(fd);
                return 1;
            }

            if (pid == 0)
            {
                if (dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("ShellForge: dup2");
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                close(fd);

                execvp(args[0], args);

                perror("ShellForge: execvp");
                exit(EXIT_FAILURE);
            }

            close(fd);

            wait_for_child(pid);

            return 1;
        }

        /*
         * Append redirection: >>
         *
         * Example:
         * echo Hello >> log.txt
         */
        if (strcmp(args[i], ">>") == 0)
        {
            int fd;
            pid_t pid;

            if (args[i + 1] == NULL)
            {
                fprintf(stderr,
                        "ShellForge: missing file after '>>'\n");
                return 1;
            }

            args[i] = NULL;

            fd = open(args[i + 1],
                      O_WRONLY | O_CREAT | O_APPEND,
                      0644);

            if (fd < 0)
            {
                perror("ShellForge: open");
                return 1;
            }

            pid = fork();

            if (pid < 0)
            {
                perror("ShellForge: fork");
                close(fd);
                return 1;
            }

            if (pid == 0)
            {
                if (dup2(fd, STDOUT_FILENO) == -1)
                {
                    perror("ShellForge: dup2");
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                close(fd);

                execvp(args[0], args);

                perror("ShellForge: execvp");
                exit(EXIT_FAILURE);
            }

            close(fd);

            wait_for_child(pid);

            return 1;
        }

        /*
         * Input redirection: <
         *
         * Example:
         * cat < input.txt
         */
        if (strcmp(args[i], "<") == 0)
        {
            int fd;
            pid_t pid;

            if (args[i + 1] == NULL)
            {
                fprintf(stderr,
                        "ShellForge: missing file after '<'\n");
                return 1;
            }

            args[i] = NULL;

            fd = open(args[i + 1], O_RDONLY);

            if (fd < 0)
            {
                perror("ShellForge: open");
                return 1;
            }

            pid = fork();

            if (pid < 0)
            {
                perror("ShellForge: fork");
                close(fd);
                return 1;
            }

            if (pid == 0)
            {
                if (dup2(fd, STDIN_FILENO) == -1)
                {
                    perror("ShellForge: dup2");
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                close(fd);

                execvp(args[0], args);

                perror("ShellForge: execvp");
                exit(EXIT_FAILURE);
            }

            close(fd);

            wait_for_child(pid);

            return 1;
        }

        /*
         * Error redirection: 2>
         *
         * Example:
         * ls nonexistent 2> error.txt
         */
        if (strcmp(args[i], "2>") == 0)
        {
            int fd;
            pid_t pid;

            if (args[i + 1] == NULL)
            {
                fprintf(stderr,
                        "ShellForge: missing file after '2>'\n");
                return 1;
            }

            args[i] = NULL;

            fd = open(args[i + 1],
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

            if (fd < 0)
            {
                perror("ShellForge: open");
                return 1;
            }

            pid = fork();

            if (pid < 0)
            {
                perror("ShellForge: fork");
                close(fd);
                return 1;
            }

            if (pid == 0)
            {
                if (dup2(fd, STDERR_FILENO) == -1)
                {
                    perror("ShellForge: dup2");
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                close(fd);

                execvp(args[0], args);

                perror("ShellForge: execvp");
                exit(EXIT_FAILURE);
            }

            close(fd);

            wait_for_child(pid);

            return 1;
        }
    }

    /*
     * No redirection operator was found.
     */
    return 0;
}
