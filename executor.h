#ifndef EXECUTOR_H
#define EXECUTOR_H

void handle_cd(char **words);
void execute(char **words, char *fileName);
void execute_pipe(char **cmd1, char **cmd2);

#endif
