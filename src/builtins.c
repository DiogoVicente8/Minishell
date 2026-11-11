/* Comandos executados pelo proprio processo da shell. */
#include "minishell.h"
#include <unistd.h>

int execute_builtin(CMD *command, int *exit_requested)
{
    if (command == NULL || command->name == NULL)
    {
        return 0;
    }

    /// Sair da shell
    if (strcmp(command->name, "exit") == 0)
    {
        *exit_requested = 1;
        return 1;
    }
    // Mudar de path (diretorio )
    if (strcmp(command->name, "cd") == 0)
    {
        char *path = command->argv[1];
        if (path == NULL)
        {
            path = getenv("HOME");
        }

        if (path == NULL)
        {
            fprintf(stderr, "cd: HOME nao definido\n");
            return 1;
        }
        if (chdir(path) != 0)
        {
            perror("cd");
        }
        return 1;
    }
    // Mostrar a diretoria atual
    if (strcmp(command->name, "pwd") == 0)
    {
        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
        else
        {
            perror("pwd");
        }
        return 1;
    }
    // Mostrar o historico
    if (strcmp(command->name, "history") == 0)
    {
        HIST_ENTRY **history = history_list();
        if (history != NULL)
        {
            for (int i = 0; history[i]; i++)
            {
                printf("%d: %s\n", i + history_base, history[i]->line);
            }
        }
        return 1;
    }
    // Mostrar texto na shell
    if (strcmp(command->name, "echo") == 0)
    {
        for (int i = 1; command->argv[i] != NULL; i++)
        {
            if (i > 1)
            {
                printf(" ");
            }
            printf("%s", command->argv[i]);
        }
        printf("\n");
        return 1;
    }
    // Mostrar ajuda
    if (strcmp(command->name, "help") == 0)
    {
        printf("Comandos internos da Minishell:\n");
        printf("cd <diretoria> - Mudar de diretoria\n");
        printf("pwd            - Mostrar diretoria atual\n");
        printf("exit           - Sair da shell\n");
        printf("history        - Mostrar historico\n");
        printf("echo <texto>   - Mostrar texto\n");
        printf("help           - Mostrar ajuda\n");

        return 1;
    }
    return 0;
}