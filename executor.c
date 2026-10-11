#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <fcntl.h>
#include "executor.h"
void handle_cd(char **words){
  if(words[1] != NULL){
    int resChdir = chdir(words[1]);
    if(resChdir <0){
      perror("Error chdir");
    }
  }

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
void execute_pipe(char **cmd1, char **cmd2){
  int fd[2];
  int result = pipe(fd);
  if(result == -1){
    perror("pipe error");
    return;
  } 
  pid_t pid1 = fork();
  if(pid1 < 0){
    perror("fork pid1 error");
    close(fd[0]);
    close(fd[1]);
    return;
  }
  if(pid1 == 0){
    dup2(fd[1],1);
    close(fd[0]);
    close(fd[1]);
    execvp(cmd1[0],cmd1);
    perror("error execvp pid1");
    exit(1);
  }
  pid_t pid2 = fork();

  if(pid2 < 0){
    perror("fork pid2 error");
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1,NULL,0);
    return;
  }
  if(pid2 == 0){
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    execvp(cmd2[0],cmd2);
    perror("error execvp pid2");
    exit(1);
  }
  close(fd[0]);
  close(fd[1]);

  waitpid(pid1,NULL,0);
  waitpid(pid2,NULL,0);
}
