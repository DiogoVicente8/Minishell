/* Redirecionamentos de entrada, saida e erros. */
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

static int redirecionar(const char *ficheiro, int flags, int destino)
{
    int fd = open(ficheiro, flags, 0644);

    if (fd == -1) {
        perror("open");
        return -1;
    }

    if (dup2(fd, destino) == -1) {
        perror("dup2");
        if (close(fd) == -1)
            perror("close");
        return -1;
    }

    if (close(fd) == -1) {
        perror("close");
        return -1;
    }
    return 0;
}

int apply_redirections(const CMD *command)
{
    int flags;
    const char *ficheiro;

    if (command == NULL)
        return -1;

    // Ler de um ficheiro: <
    if (command->infile != NULL) {
        if (redirecionar(command->infile, O_RDONLY, STDIN_FILENO) == -1)
            return -1;
    }

    // Escrever num ficheiro: > ou >>
    if (command->outfile != NULL) {
        ficheiro = command->outfile;
        flags = O_WRONLY | O_CREAT;

        // O parser original deixa um '>' no inicio para indicar >>
        if (ficheiro[0] == '>') {
            ficheiro++;
            flags |= O_APPEND;
        }
        else {
            flags |= O_TRUNC;
        }

        if (*ficheiro == '\0') {
            fprintf(stderr, "Falta o nome do ficheiro\n");
            return -1;
        }

        if (redirecionar(ficheiro, flags, STDOUT_FILENO) == -1)
            return -1;
    }

    // Guardar mensagens de erro: 2>
    if (command->errfile != NULL) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        if (redirecionar(command->errfile, flags, STDERR_FILENO) == -1)
            return -1;
    }

    return 0;
}