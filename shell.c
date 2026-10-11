#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <fcntl.h>
int split_line(char *line, char **words){

  bool between_words = true;
  int counter = 0;
  for (int i = 0; line[i] != '\0'; i++) {

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
  return counter;
}
void discard_rest_of_line(void){
  int c = getchar();
  while(c != '\n' && c != EOF){
    c = getchar();
  }

}
void handle_cd(char **words){
  if(words[1] != NULL){
    int resChdir = chdir(words[1]);
    if(resChdir <0){
      perror("Error chdir");
    }
  }

}
bool find_redirection(char **words, int counter, char **fileName){
  for(int i = 0; i< counter; i++){
    if(strcmp(words[i],">") == 0){
      if(words[i+1] == NULL){
        fprintf(stderr,"No file name\n");
        return false;
      }
      *fileName =  words[i+1];
      words[i] = NULL ;
      break;
    }
  }
  return true;

}
void execute(char ** words, char *fileName){
  pid_t pid = fork();
  if(pid <0){
    perror("Error fork");
    return;
  } else if(pid ==0){
    if(fileName != NULL){
      int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if(fd < 0){
        perror("Error open");
        exit(1);
      } else {
        dup2(fd,1);
        close(fd);
      }
    }
    execvp(words[0],words);
    perror("Error execvp");
    exit(1);
  } else {
    waitpid(pid,NULL,0);
  }

}
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
    execute(words,fileName);

  }
  return 0;
}
