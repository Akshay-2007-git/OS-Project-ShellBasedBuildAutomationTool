#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build_tool.h"
#include "input.h"
#include "parser.h"
#include "builtin.h"
#include "process.h"

int main()
{
    char *line;
    char **tokens;

    printf("============================================\n");
    printf("   %s\n", TOOL_NAME);
    printf("============================================\n");

    printf("Type 'help' to see available commands.\n\n");

    while (1)
    {
        printf("build> ");

        line = read_line();

        if (line == NULL)
        {
            break;
        }

        /*
         * Ignore empty input.
         */
        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        /*
         * WEEK 3
         *
         * Convert the input string into tokens.
         */
        tokens = parse_line(line);

        if (tokens == NULL || tokens[0] == NULL)
        {
            free_tokens(tokens);
            free(line);
            continue;
        }

        /*
         * WEEK 5
         *
         * Check whether the command is a built-in.
         */
        int builtin_result = execute_builtin(tokens);

        /*
         * exit was requested.
         */
        if (builtin_result == -1)
        {
            free_tokens(tokens);
            free(line);
            break;
        }

        /*
         * Not a built-in.
         *
         * WEEK 4
         *
         * Execute the command using fork(),
         * execvp(), and waitpid().
         */
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
