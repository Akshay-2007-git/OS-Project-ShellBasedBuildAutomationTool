#include <stdio.h>
#include <string.h>
#include "build_tool.h"

int main()
{
    char input[MAX_INPUT];

    printf("============================================\n");
    printf("   Shell-Based Build Automation Tool\n");
    printf("============================================\n");
    printf("Type 'help' to see available commands.\n\n");

    while (1)
    {
        printf("build> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting Build Automation Tool...\n");
            break;
        }

        else if (strcmp(input, "help") == 0)
        {
            printf("\nAvailable commands:\n");
            printf("  help   - Show available commands\n");
            printf("  build  - Build the project\n");
            printf("  clean  - Remove generated files\n");
            printf("  status - Show project status\n");
            printf("  exit   - Exit the tool\n\n");
        }

        else if (strcmp(input, "build") == 0)
        {
            printf("Build command received.\n");
            printf("Build functionality will be implemented in future weeks.\n");
        }

        else if (strcmp(input, "clean") == 0)
        {
            printf("Clean command received.\n");
            printf("Clean functionality will be implemented in future weeks.\n");
        }

        else if (strcmp(input, "status") == 0)
        {
            printf("Status command received.\n");
            printf("Status functionality will be implemented in future weeks.\n");
        }

        else if (strlen(input) == 0)
        {
            continue;
        }

        else
        {
            printf("Unknown command: %s\n", input);
            printf("Type 'help' for available commands.\n");
        }
    }

    return 0;
}
