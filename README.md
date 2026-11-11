# Minishell em C — Estrutura inicial da Fase 1

Projeto de Sistemas Operativos, **apenas Fase 1 (10 valores)**. Os ficheiros `src/parse.c` e `include/header.h` foram mantidos a partir do `parse.zip` do docente. O `main.c` foi adaptado para encaminhar os comandos para os modulos de execucao. O projeto e um **esqueleto**, nao a solucao da fase.

## Estrutura

```text
Minishell_Fase1/
├── src/
│   ├── main.c         # REPL: leitura, parser e libertacao da memoria
│   ├── parse.c        # Parser original do docente
│   ├── execute.c      # Execucao de comandos externos e coordenacao
│   ├── builtins.c     # cd, pwd, exit, history, echo, help
│   ├── redirect.c     # <, >, >>, 2>
│   ├── pipes.c        # Comandos ligados por pipes
│   └── signals.c      # SIGCHLD, SIGINT, SIGTSTP
├── include/
│   ├── header.h       # Estrutura CMD original do docente
│   └── minishell.h    # Prototipos dos modulos
├── tests/
│   └── testes_fase1.md
├── Makefile
├── .gitignore
└── README.md
```

## Requisitos

Ubuntu/Linux, `gcc`, `make` e GNU Readline. Em Ubuntu:

```bash
sudo apt install build-essential libreadline-dev
```

## Compilar e executar

```bash
make
./minishell
```

Para limpar: `make clean`.

## O que funciona agora

- Prompt `msh$` com GNU Readline.
- Interpretacao inicial da linha pelo parser do docente.
- Libertacao da lista ligada e da entrada lida.
- `exit` e Ctrl+D para sair.

**Nota:** os comandos `ls`, `pwd`, `cd`, pipes, redirecionamentos, background e tratamento de sinais **ainda nao estao implementados**. Os respetivos modulos contem `TODO`.

## Ordem recomendada de implementacao (Fase 1)

1. `execute.c`: `fork()`, `execvp()` e `waitpid()`.
2. `builtins.c`: `cd`, `pwd`, `history`, `echo`, `help` (o `exit` basico ja existe).
3. Adaptar o parser e executor para `&` (background).
4. `redirect.c`: `open()`, `dup2()`, `close()`, incluindo `>>`.
5. `pipes.c`: ligar multiplos processos com `pipe()` e `dup2()`.
6. `signals.c`: `SIGCHLD`, `SIGINT` e `SIGTSTP`; testar memoria e erros.

### Limitacoes do parser fornecido

O parser original separa sobretudo os argumentos por espacos. Ainda nao fornece os indicadores necessarios para distinguir `>>` e `&`; sera necessario adaptar o parser e/ou a estrutura de comandos. Ao expandir o parser, manter a gestao correta da memoria.

## Compilacao

O Makefile usa `-Wall -Wextra -pedantic -pthread -std=c11` como pede o enunciado.
