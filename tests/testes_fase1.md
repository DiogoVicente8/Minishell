# Testes incrementais da Fase 1

Assinala cada teste quando tiveres implementado o requisito correspondente.

- [-] `ls -l` — comando externo simples
- [-] `cd ..` seguido de `pwd` — diretorio do processo pai
- [-] `history` — historico
- [-] `echo ola` — builtin
- [-] `sleep 3 &` — background (deve devolver o prompt imediatamente)
- [-] `echo ola > teste.txt` — redirecionamento de stdout
- [-] `echo mundo >> teste.txt` — acrescentar ao ficheiro
- [-] `cat < teste.txt` — entrada redirecionada
- [-] `ls /naoexiste 2> erros.txt` — erros redirecionados
- [-] `ls | wc -l` — pipe simples
- [-] `cat teste.txt | grep ola | wc -l` — pipeline longo
- [-] Fechar com Ctrl+D
- [-] Ctrl+C nao deve matar a shell
- [ ] Verificar fugas de memoria com Valgrind
