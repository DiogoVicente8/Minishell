# Minishell em C 

Projeto prático da unidade curricular de **Sistemas Operativos**

Os ficheiros `src/parse.c` e `include/header.h`, fornecidos pelo docente, foram mantidos sem alterações. A preparação dos operadores é feita em `src/main.c` para permitir a utilização do parser original.

## Requisitos

- Ubuntu/Linux
- GCC e Make
- Biblioteca GNU Readline

## Compilação e execução

Na pasta principal do projeto:

```bash
make
./minishell
```

A Minishell apresenta o prompt `msh$`. Para sair, escrever `exit` ou carregar em **Ctrl+D**.

## Funcionalidades implementadas

### Comandos internos

- `cd [diretoria]` — muda de diretoria; sem argumento, usa `HOME`.
- `pwd` — apresenta a diretoria atual.
- `exit` — termina a Minishell.
- `history` — mostra o histórico de comandos da sessão.
- `echo [texto]` — escreve os argumentos no terminal.
- `help` — apresenta a ajuda.

Os comandos internos simples são executados no processo da shell. Em pipelines e em background, são executados em processos filhos.

### Comandos externos

São executados usando `fork()` e `execvp()`. No foreground, a Minishell espera pela conclusão com `waitpid()`.

```text
msh$ ls -l
msh$ date
msh$ whoami
```

### Background (`&`)

O operador `&` no fim da linha executa os comandos sem bloquear o prompt e apresenta o PID do primeiro processo.

```text
msh$ sleep 5 &
msh$ sleep 5 | cat &
```

O sinal `SIGCHLD` é utilizado para recolher processos terminados. Na ausência de redirecionamento de entrada, o primeiro comando em background lê de `/dev/null`.

### Redirecionamentos

| Operador | Função |
|---|---|
| `<` | Entrada a partir de um ficheiro |
| `>` | Saída para um ficheiro, substituindo o conteúdo |
| `>>` | Saída acrescentada ao fim do ficheiro |
| `2>` | Saída de erro para um ficheiro |

```text
msh$ echo Ola > teste.txt
msh$ echo Mundo >> teste.txt
msh$ cat < teste.txt
msh$ ls pasta_inexistente 2> erros.txt
```

Os redirecionamentos também são aplicáveis a comandos internos, como `echo`.

### Pipes (`|`)

É possível ligar vários comandos num pipeline. O código cria um filho por comando e liga a entrada e a saída com `pipe()` e `dup2()`.

```text
msh$ ls | wc -l
msh$ ls | sort | wc -l
msh$ cat < teste.txt | sort | uniq > resultado.txt
```

### Sinais

- **SIGINT (Ctrl+C):** a shell ignora o sinal; os filhos em foreground usam o comportamento normal do sinal.
- **SIGTSTP (Ctrl+Z):** ignorado pela shell e pelos filhos nesta versão. Não existe suspensão de jobs.
- **SIGCHLD:** permite recolher processos filhos terminados.

O `sigprocmask()` evita que o tratamento de `SIGCHLD` interfira com o `waitpid()` de comandos em foreground.

## Organização do projeto

```text
Minishell/
├── src/
│   ├── main.c         # Ciclo principal e preparação da linha
│   ├── parse.c        # Parser original do docente
│   ├── builtins.c     # Comandos internos
│   ├── execute.c      # Escolha do tipo de execução
│   ├── pipes.c        # Processos, pipelines e background
│   ├── redirect.c     # Redirecionamentos
│   └── signals.c      # Sinais
├── include/
│   ├── header.h       # Estrutura CMD original do docente
│   └── minishell.h    # Protótipos das funções
├── tests/
│   ├── Test.md
│   └── testes_fase1.md
├── Makefile
├── README.md
└── .gitignore
```

## Testes

Os comandos de verificação encontram-se em [tests/testes_fase1.md](tests/testes_fase1.md).

## Limitações conhecidas

- O parser original separa principalmente os argumentos por espaços. Não interpreta completamente aspas, escapes, variáveis, expansão de `~` ou padrões como `*.c`.
- Não são suportados operadores como `&&`, `||` e `;`.
- O `&` é reconhecido no fim da linha.
- O número de argumentos é limitado por `MAXARGS`, definido no `header.h`.
- Não existe controlo completo de jobs: `Ctrl+Z` não suspende processos e o background não utiliza grupos de processos separados.
- Alguns comandos malformados, como uma linha iniciada por `|`, não apresentam uma mensagem de erro específica.

