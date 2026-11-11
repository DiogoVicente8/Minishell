
    
  
/* Processos e comunicacao por pipes. */
#include "minishell.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

// Fechar um descritor e avisar se houver erro
static void fechar(int fd)
{
    if (fd != -1 && close(fd) == -1)
        perror("close");
}

int execute_pipeline(CMD *commands, int background)
{
    CMD *cmd;
    int total = 0, criados = 0;
    int anterior = -1, erro = 0, ultimo_estado = 0;
    pid_t pid, *pids;
    sigset_t bloqueados, mascara_antiga;

    // Contar os comandos
    for (cmd = commands; cmd != NULL; cmd = cmd->next) {
        if (cmd->name == NULL || cmd->argv[0] == NULL) {
            fprintf(stderr, "Comando vazio no pipe\n");
            return -1;
        }
        total++;
    }

    if (total == 0)
        return 0;

    pids = malloc(total * sizeof(pid_t));
    if (pids == NULL) {
        perror("malloc");
        return -1;
    }

    // Evitar que o handler de SIGCHLD recolha os filhos do foreground
    if (sigemptyset(&bloqueados) == -1 ||
        sigaddset(&bloqueados, SIGCHLD) == -1) {
        perror("sigset");
        free(pids);
        return -1;
    }
    if (sigprocmask(SIG_BLOCK, &bloqueados, &mascara_antiga) == -1) {
        perror("sigprocmask");
        free(pids);
        return -1;
    }

    for (cmd = commands; cmd != NULL; cmd = cmd->next) {
        int fd[2] = {-1, -1};

        // So criamos pipe quando existe um proximo comando
        if (cmd->next != NULL && pipe(fd) == -1) {
            perror("pipe");
            erro = 1;
            break;
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            fechar(fd[0]);
            fechar(fd[1]);
            erro = 1;
            break;
        }

        if (pid == 0) {
            int sair = 0;

            // O Ctrl+C termina comandos em foreground, mas nao a shell
            if (signal(SIGINT, background ? SIG_IGN : SIG_DFL) == SIG_ERR ||
                signal(SIGTSTP, SIG_IGN) == SIG_ERR) {
                perror("signal");
                _exit(1);
            }

            // O filho nao deve ficar com o SIGCHLD bloqueado
            if (sigprocmask(SIG_SETMASK, &mascara_antiga, NULL) == -1) {
                perror("sigprocmask");
                _exit(1);
            }

            // Receber dados do comando anterior
            if (anterior != -1 && dup2(anterior, STDIN_FILENO) == -1) {
                perror("dup2");
                _exit(1);
            }

            // Enviar dados para o comando seguinte
            if (fd[1] != -1 && dup2(fd[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                _exit(1);
            }

            fechar(anterior);
            fechar(fd[0]);
            fechar(fd[1]);

            // Um comando em background nao le do teclado
            if (background && cmd == commands && cmd->infile == NULL) {
                int entrada = open("/dev/null", O_RDONLY);
                if (entrada == -1 || dup2(entrada, STDIN_FILENO) == -1) {
                    perror("/dev/null");
                    _exit(1);
                }
                fechar(entrada);
            }

            if (apply_redirections(cmd) == -1)
                _exit(1);

            // Comandos internos num pipe sao executados pelo filho
            if (execute_builtin(cmd, &sair)) {
                fflush(stdout);
                _exit(0);
            }

            execvp(cmd->name, cmd->argv);
            perror(cmd->name);
            _exit(127);
        }

        // O pai guarda o PID e fecha o que ja nao usa
        pids[criados++] = pid;
        fechar(anterior);
        fechar(fd[1]);
        anterior = fd[0];
    }

    fechar(anterior);

    if (background && !erro) {
        printf("[Processo em background iniciado com PID %ld]\n", (long)pids[0]);
        fflush(stdout);
    } else {
        // No foreground, esperar por todos os filhos
        for (int i = 0; i < criados; i++) {
            int estado;
            pid_t resultado;

            do {
                resultado = waitpid(pids[i], &estado, 0);
            } while (resultado == -1 && errno == EINTR);

            if (resultado == -1) {
                perror("waitpid");
                erro = 1;
            } else if (i == criados - 1) {
                ultimo_estado = estado;
            }
        }
    }

    // Voltar a permitir SIGCHLD no pai
    if (sigprocmask(SIG_SETMASK, &mascara_antiga, NULL) == -1) {
        perror("sigprocmask");
        erro = 1;
    }
    free(pids);

    if (erro) return -1;
    if (background) return 0;
    if (WIFEXITED(ultimo_estado)) return WEXITSTATUS(ultimo_estado);
    if (WIFSIGNALED(ultimo_estado)) return 128 + WTERMSIG(ultimo_estado);
    return -1;
}
