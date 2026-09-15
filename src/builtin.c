#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "build_tool.h"
#include "process.h"

int execute_builtin(char **args)
{
    if (args == NULL || args[0] == NULL)
    {
        return 1;
    }

    /*
     * exit
     */
    if (strcmp(args[0], "exit") == 0)
    {
        return -1;
    }

    /*
     * pwd
     */
    if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[1024];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
        else
        {
            perror("ShellForge: pwd");
        }

        return 1;
    }

    /*
     * cd
     */
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            printf("Usage: cd <directory>\n");
            return 1;
        }

        if (chdir(args[1]) != 0)
        {
            perror("ShellForge: cd");
        }

        return 1;
    }

    /*
     * help
     */
    if (strcmp(args[0], "help") == 0)
    {
        printf("\nShellForge Commands:\n");
        printf("--------------------\n");
        printf("build <command>   Execute a build command\n");
        printf("pwd               Show current directory\n");
        printf("cd <directory>    Change directory\n");
        printf("env               Show environment variables\n");
        printf("clear             Clear the terminal\n");
        printf("help              Show this help message\n");
        printf("exit              Exit ShellForge\n\n");

        return 1;
    }

    /*
     * clear
     */
    if (strcmp(args[0], "clear") == 0)
    {
        printf("\033[H\033[J");
        return 1;
    }

    /*
     * Environment variables
     */
    if (strcmp(args[0], "env") == 0)
    {
        char *home = getenv("HOME");
        char *user = getenv("USER");
        char *path = getenv("PATH");

        printf("HOME = %s\n", home ? home : "Not set");
        printf("USER = %s\n", user ? user : "Not set");
        printf("PATH = %s\n", path ? path : "Not set");

        return 1;
    }

    /*
     * build command
     *
     * build gcc main.c -o app
     */
    if (strcmp(args[0], "build") == 0)
    {
        if (args[1] == NULL)
        {
            printf("Usage: build <command> [arguments...]\n");
            return 1;
        }

        /*
         * Move one position forward.
         *
         * args[1] becomes the program executed by execvp().
         */

        execute_process(&args[1]);

        return 1;
    }

    /*
     * Not a built-in command.
     */
    return 0;
}
