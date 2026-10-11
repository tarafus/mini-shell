#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

int split_line(char *line, char **words);
void discard_rest_of_line(void);
bool find_redirection(char **words, int counter, char **fileName);
int find_pipe(char **words, int counter);

#endif
