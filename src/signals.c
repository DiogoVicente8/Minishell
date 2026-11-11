/* Sinais da Minishell. */
#include "minishell.h"
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

// Recolher os processos filhos que terminam em background
static void tratar_sigchld(int sinal)
{
    int erro = errno;
    (void)sinal;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = erro;
}

void setup_shell_signals(void)
{
    struct sigaction sa = {0};

    sa.sa_handler = tratar_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        perror("sigaction SIGCHLD");

    // O Ctrl+C e Ctrl+Z nao devem fechar ou suspender a shell
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
        perror("sigaction SIGINT");
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
        perror("sigaction SIGTSTP");
}