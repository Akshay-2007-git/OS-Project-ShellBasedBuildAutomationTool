#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"
#include "../include/parser.h"
#include "../include/process.h"
#include "../include/builtin.h"
#include "../include/signals.h"
#include "../include/pipes.h"
#include "../include/redirect.h"
#include "../include/thread.h"

static void tokenize(char *str, char **argv)
{
    int i = 0;

    char *token = strtok(str, " \t\n");

    while (token != NULL)
    {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    argv[i] = NULL;
}

int main()
{
    char *line;
    char **tokens;
    printf("\n");
    printf("========================================\n");
    printf("   Shell-Based Build Automation Tool\n");
    printf("========================================\n");
    printf("\n");
    initialize_signals();

    while (1)
    {
        printf("myshell> ");
        fflush(stdout);

        line = read_line();

        if (line == NULL)
        {
            break;
        }

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            break;
        }

        /*
         * Week 10:
         * Run the build automation task
         * using a separate thread and monitor
         * its execution using another thread.
         */
        if (strcmp(line, "build") == 0)
        {
            start_build_monitor();
            start_build_task();

            wait_for_build_task();
            stop_build_monitor();

            free(line);
            continue;
        }

        /*
         * Pipe handling
         */
        if (strchr(line, '|') != NULL)
        {
            char *argv1[64];
            char *argv2[64];

            char *left = strtok(line, "|");
            char *right = strtok(NULL, "|");

            if (left == NULL || right == NULL)
            {
                printf("Invalid pipe command\n");
                free(line);
                continue;
            }

            tokenize(left, argv1);
            tokenize(right, argv2);

            execute_pipe(argv1, argv2);
        }
        else
        {
            /*
             * Normal command processing
             */
            tokens = parse_line(line);

            if (execute_builtin(tokens) == 0)
            {
                if (execute_redirection(tokens) == 0)
                {
                    execute_process(tokens);
                }
            }

            free_tokens(tokens);
        }

        free(line);
    }

    return 0;
}
