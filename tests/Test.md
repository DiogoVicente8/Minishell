# Testes

- pipes: `ls | grep c | wc -l`
- redirects: `ls > out.txt`, `ls >> out.txt`, `wc < out.txt`, `ls nao_existe 2> err.txt`
- background: `sleep 5 &`
- valgrind: `valgrind --leak-check=full --track-fds=yes ./minishell`
