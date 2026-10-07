#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
char line[100];
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
      int c = getchar();
      while(c != '\n' && c != EOF){
        c = getchar();
      }
      continue;
    }
    bool between_words = true;
    char *words[50];
    int counter = 0;
    for (int i = 0; i < sizeof(line); i++) {
      
      if(line[i] == '\0'){
        break;
      } else if(line[i] == ' ' || line[i] == '\n') {
        between_words = true;      
        line[i] = '\0';
      } else {
        if(between_words){
          words[counter] = &line[i];
          counter++;
        }
        between_words = false;
      }
    }
    words[counter] = NULL;
    if(counter ==0) continue;

    if(strcmp(words[0],"exit") == 0) break;
    if(strcmp(words[0],"cd") == 0) {
      if(words[1] != NULL){
        int resChdir = chdir(words[1]);
        if(resChdir <0){
          perror("Error chdir");
        }
      }
      continue;
    }
    pid_t pid = fork();
    if(pid <0){
      perror("Error fork");
      continue;
    } else if(pid ==0){
      execvp(words[0],words);
      perror("Error execvp");
      exit(1);
    } else {
      waitpid(pid,NULL,0);
    }
  }
  return 0;
}
