# Testes da Fase 1

Guia de testes manuais para verificar a Minishell antes da entrega.

Na raiz do projeto, executar `make clean && make` e `./minishell`. Introduzir os comandos **um de cada vez** no prompt `msh$`. As caixas devem ser assinaladas após confirmação no computador.

## 1. Comandos internos e externos

- [✓] `pwd` — mostra a diretoria atual.
- [✓] `cd /tmp` seguido de `pwd` — mostra `/tmp`.
- [✓] `cd` seguido de `pwd` — volta à diretoria HOME.
- [✓] `echo Ola Mundo` — apresenta `Ola Mundo`.
- [✓] `help` — mostra a ajuda.
- [✓] `history` — apresenta os comandos da sessão.
- [✓] `ls -l` — executa um comando externo.
- [✓] `comando_inexistente` — apresenta erro e a shell continua aberta.

## 2. Redirecionamentos

- [✓] `echo Ola > teste.txt` e `cat teste.txt` — o ficheiro contém `Ola`.
- [✓] `echo Mundo >> teste.txt` e `cat teste.txt` — o ficheiro contém duas linhas.
- [✓] `wc -l < teste.txt` — apresenta `2`.
- [✓] `ls /diretoria_inexistente 2> erros.txt` e `cat erros.txt` — a mensagem de erro foi guardada.
- [✓] `echo Teste > builtin.txt` e `cat builtin.txt` — testa um redirecionamento com comando interno.
- [✓] `echo Terminal` — a saída continua a funcionar depois dos redirecionamentos.

## 3. Pipes

- [✓] `printf abc | wc -c` — apresenta `3`.
- [✓] `printf abc|wc -c` — apresenta `3` sem espaços junto ao pipe.
- [✓] `ls | sort | wc -l` — executa três comandos.
- [✓] `ls | sort | uniq | wc -l` — executa quatro comandos.
- [✓] `cat < teste.txt | sort | uniq > resultado.txt` — cria o ficheiro de saída.
- [✓] Repetir `printf abc | wc -c` várias vezes — não bloqueia nem apresenta erros de `waitpid()`.

## 4. Background

- [✓] `sleep 5 &` — mostra um PID e devolve imediatamente o prompt.
- [✓] `sleep 5 | cat &` — o pipeline devolve o prompt imediatamente.
- [✓] Executar `pwd` após `sleep 5 &` — a shell continua a aceitar comandos.

## 5. Sinais e saída

- [✓] `sleep 30` e **Ctrl+C** — termina o comando e mantém a shell aberta.
- [✓] `sleep 30 | cat` e **Ctrl+C** — interrompe o pipeline.
- [✓] `sleep 30` e **Ctrl+Z** — nesta versão o sinal é ignorado; usar **Ctrl+C** para terminar o teste.
- [✓] **Ctrl+D** no prompt vazio — termina a Minishell.
- [✓] `exit` — termina a Minishell.

## 7. Casos-limite

- [✓] `cat < ficheiro_inexistente` — apresenta erro e a shell continua.
- [✓] `echo Ola >` — informa que falta o nome do ficheiro.
- [✓] `ls |` — apresenta erro para comando vazio no pipeline.
- [✓] `| wc -l` — **limitação conhecida**: pode não apresentar diagnóstico; a shell não deve terminar.
