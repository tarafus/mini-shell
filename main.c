#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "parser.h"
#include "executor.h"
int main(int argc, char *argv[])
{
  char line[100];
  char *words[50];
  while (true) {
    printf("> "); 
    char * result = fgets(line,sizeof(line),stdin);
    if(result == NULL){
      break;
    } else if(strcmp(line,"exit\n") == 0){
      break;
    }
    char * p = strchr(line, '\n');
    if(p == NULL){
      fprintf(stderr, "ligne trop longue\n");
      discard_rest_of_line();
      continue;
    }
    int counter = split_line(line,words);
    if(counter ==0) continue;

    if(strcmp(words[0],"exit") == 0) break;
    if(strcmp(words[0],"cd") == 0) {
      handle_cd(words);
      continue;
    }

    char * fileName = NULL;
    if(!find_redirection(words, counter, &fileName)) continue;
    int pipePos = find_pipe(words, counter);
    if (pipePos == -2) continue;
    if (pipePos >= 0) {
      execute_pipe(words, &words[pipePos + 1]);
      continue;
    }
    execute(words,fileName);

  }
  return 0;
}
