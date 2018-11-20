#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char ** parse_args(char *line){
  char ** arr = calloc(sizeof(char*), 5);
  int i = 0;
  while (line) {
    arr[i] = strsep(&line, " ");
    i++;
  }
  return arr;
}

void start(){
  while(1){
    char * input = malloc(100);
    fgets(input,100,stdin);
    input[strlen(input)-1] = 0;
    char ** line = parse_args(input);
    //printf("%s \n", commands[0]);
    //char bin[50] = "/bin/";
    //char * path = strcat(bin, commands[0]);
    //printf("%s \n", path);
    int next = fork();
    if(! next){
      execvp(line[0], line);
    }
  }
}

int main(){
  start();
}
