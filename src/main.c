#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build_tool.h"
#include "input.h"
#include "parser.h"
#include "builtin.h"
#include "process.h"
#include "signals.h"
#include "pipes.h"

#define PIPE_TOKEN_SIZE 64

static void tokenize_pipe_command(char *command, char **tokens)
{
    int position = 0;

    char *token = strtok(command, " \t\r\n\a");

    while (token != NULL && position < PIPE_TOKEN_SIZE - 1)
    {
        tokens[position] = token;
        position++;

        token = strtok(NULL, " \t\r\n\a");
    }

    tokens[position] = NULL;
}

int main(void)
{
    char *line;
    char **tokens;

    initialize_signals();

    printf("============================================\n");
    printf("   %s\n", TOOL_NAME);
    printf("============================================\n");

    printf("Type 'help' to see available commands.\n\n");

    while (1)
    {
        printf("build> ");
        fflush(stdout);

        line = read_line();

        if (line == NULL)
        {
            break;
        }

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        /*
         * Check whether the user entered a pipe.
         */
        if (strchr(line, '|') != NULL)
        {
            char *left_command;
            char *right_command;
            char **pipe_tokens1;
            char **pipe_tokens2;

            pipe_tokens1 = malloc(PIPE_TOKEN_SIZE * sizeof(char *));
            pipe_tokens2 = malloc(PIPE_TOKEN_SIZE * sizeof(char *));

            if (pipe_tokens1 == NULL || pipe_tokens2 == NULL)
            {
                fprintf(stderr, "Memory Allocation Failed\n");

                free(pipe_tokens1);
                free(pipe_tokens2);
                free(line);

                continue;
            }

            /*
             * Split the input around the pipe.
             */
            left_command = strtok(line, "|");
            right_command = strtok(NULL, "|");

            /*
             * Only one pipe is supported in Week 7.
             */
            if (left_command == NULL ||
                right_command == NULL ||
                strchr(right_command, '|') != NULL)
            {
                printf("Invalid pipe command\n");

                free(pipe_tokens1);
                free(pipe_tokens2);
                free(line);

                continue;
            }

            tokenize_pipe_command(left_command, pipe_tokens1);
            tokenize_pipe_command(right_command, pipe_tokens2);

            if (pipe_tokens1[0] == NULL ||
                pipe_tokens2[0] == NULL)
            {
                printf("Invalid pipe command\n");

                free(pipe_tokens1);
                free(pipe_tokens2);
                free(line);

                continue;
            }

            execute_pipe(pipe_tokens1, pipe_tokens2);

            free(pipe_tokens1);
            free(pipe_tokens2);
            free(line);

            continue;
        }

        /*
         * Normal non-pipe command.
         */
        tokens = parse_line(line);

        if (tokens == NULL || tokens[0] == NULL)
        {
            free_tokens(tokens);
            free(line);
            continue;
        }

        int builtin_result = execute_builtin(tokens);

        if (builtin_result == -1)
        {
            free_tokens(tokens);
            free(line);
            break;
        }

        if (builtin_result == 0)
        {
            execute_process(tokens);
        }

        free_tokens(tokens);
        free(line);
    }

    printf("\nGoodbye!\n");

    return 0;
}
