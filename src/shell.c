#include "shell.h"


/* shell.c - REPL (req. 1.1)
 * TODO: prompt, readline, add_history, parse_line, executor_run,
 *       free_command_list e free(line) no fim de cada iteracao */

void shell_run(void)
 {
     char *line;
        t_command_list *command_list;

     while (1)
     {
         line = readline("minishell> ");
         if (!line)
             break;
         add_history(line);
         command_list = parse_line(line);
         executor_run(command_list);
         free_command_list(command_list);
         free(line);
     }
 }