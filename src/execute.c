/* Decidir se executamos um comando interno, externo ou um pipe. */
#include "minishell.h"
#include <unistd.h>

static int e_interno(const char *nome)
{
    return strcmp(nome, "cd") == 0 ||
           strcmp(nome, "pwd") == 0 ||
           strcmp(nome, "exit") == 0 ||
           strcmp(nome, "history") == 0 ||
           strcmp(nome, "echo") == 0 ||
           strcmp(nome, "help") == 0;
}

/* Os internos correm no pai, por isso temos de recuperar o stdin/stdout/stderr. */
static void builtin_com_redirecionamento(CMD *cmd, int *sair)
{
    int guardados[3];
    int i, ok = 1;

    fflush(stdout);
    fflush(stderr);

    for (i = 0; i < 3; i++)
        guardados[i] = -1;

    // Guardar os descritores atuais
    for (i = 0; i < 3; i++) {
        guardados[i] = dup(i);
        if (guardados[i] == -1) {
            perror("dup");
            ok = 0;
            break;
        }
    }

    if (ok && apply_redirections(cmd) == 0)
        execute_builtin(cmd, sair);

    fflush(stdout);
    fflush(stderr);

    // Voltar a ligar a entrada/saida ao terminal
    for (i = 0; i < 3; i++) {
        if (guardados[i] != -1) {
            if (dup2(guardados[i], i) == -1)
                perror("dup2");
            if (close(guardados[i]) == -1)
                perror("close");
        }
    }
}

int execute_command_list(CMD *commands, int background)
{
    int sair = 0;

    if (commands == NULL || commands->name == NULL)
        return 0;

    // Pipes e comandos em background precisam de processos filhos
    if (commands->next != NULL || background) {
        execute_pipeline(commands, background);
        return 0;
    }

    if (e_interno(commands->name)) {
        if (commands->infile || commands->outfile || commands->errfile)
            builtin_com_redirecionamento(commands, &sair);
        else
            execute_builtin(commands, &sair);

        return sair;
    }

    execute_external(commands);
    return 0;
}

int execute_external(CMD *command)
{
    // A mesma funcao executa um comando ou varios ligados por pipes.
    // Com um comando, nao e criado nenhum pipe.
    return execute_pipeline(command, 0);
}