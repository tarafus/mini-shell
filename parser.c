#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "parser.h"
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

int find_pipe(char **words, int counter){
  for(int i = 0; i< counter; i++){
    if(words[i] == NULL) continue;
    if(strcmp(words[i],"|") == 0){
      if(i == 0){
        fprintf(stderr,"No first command\n");
        return -2;
      }
      if(words[i+1] == NULL){
        fprintf(stderr,"No command\n");
        return -2;/*bad command*/
      }
      words[i] = NULL ;
      return i;
    }
  }
  return -1; /*no pipe*/
}
