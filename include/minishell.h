#ifndef MINISHELL_H
#define MINISHELL_H

#include "header.h"

int execute_command_list(CMD *commands, int background);
int execute_builtin(CMD *command, int *exit_requested);
int execute_external(CMD *command);
int execute_pipeline(CMD *commands, int background);
int apply_redirections(const CMD *command);
void setup_shell_signals(void);

#endif