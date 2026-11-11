/* Ciclo principal da shell. */
#include "minishell.h"
#include <ctype.h>

/*
 * O parser do professor nao separa todos os operadores.
 * Esta funcao prepara o texto, sem modificar parse.c nem header.h.
 */
static char *preparar_linha(const char *linha, int *background)
{
    size_t n = strlen(linha);
    size_t i = 0, j = 0;
    char *nova;

    *background = 0;

    // Verificar se termina com &
    while (n > 0 && isspace((unsigned char)linha[n - 1]))
        n--;

    if (n > 0 && linha[n - 1] == '&') {
        *background = 1;
        n--;
    }

    // Precisamos de espaco extra para separar os operadores
    nova = malloc(n * 4 + 4);
    if (nova == NULL)
        return NULL;

    while (i < n) {
        int tamanho = 0;

        // Separar o pipe: ls|wc passa a ls | wc
        if (linha[i] == '|') {
            nova[j++] = ' ';
            nova[j++] = '|';
            nova[j++] = ' ';
            i++;
            continue;
        }

        // Operadores de redirecionamento
        if (linha[i] == '2' && i + 1 < n && linha[i + 1] == '>' &&
            (i == 0 || isspace((unsigned char)linha[i - 1]) || linha[i - 1] == '|'))
            tamanho = 2;
        else if (linha[i] == '>' && i + 1 < n && linha[i + 1] == '>')
            tamanho = 2;
        else if (linha[i] == '>' || linha[i] == '<')
            tamanho = 1;

        if (tamanho > 0) {
            nova[j++] = ' ';

            while (tamanho-- > 0)
                nova[j++] = linha[i++];

            // Juntar o nome do ficheiro ao operador
            while (i < n && isspace((unsigned char)linha[i]))
                i++;

            if (i == n || linha[i] == '|' || linha[i] == '<' || linha[i] == '>') {
                fprintf(stderr, "Falta o nome do ficheiro\n");
                free(nova);
                return NULL;
            }

            while (i < n && !isspace((unsigned char)linha[i]) &&
                   linha[i] != '|' && linha[i] != '<' && linha[i] != '>')
                nova[j++] = linha[i++];

            nova[j++] = ' ';
        }
        else {
            nova[j++] = linha[i++];
        }
    }

    nova[j] = '\0';
    return nova;
}

int main(void)
{
    char *linha, *preparada;
    int sair = 0;

    setup_shell_signals();

    while (!sair) {
        int background = 0;

        linha = readline("msh$ ");
        if (linha == NULL) {
            putchar('\n');
            break;
        }

        if (strspn(linha, " \t\n") == strlen(linha)) {
            free(linha);
            continue;
        }

        add_history(linha);
        preparada = preparar_linha(linha, &background);

        if (preparada != NULL) {
            parse_line(preparada);

            if (command_list != NULL)
                sair = execute_command_list(command_list, background);

            free_command_list();
            free(preparada);
        }

        free(linha);
    }

    clear_history();
    return 0;
}