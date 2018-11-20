#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
    if (strcmp(line[0],"cd") == 0){
	int z = chdir(line[1]);
	if (z < 0){
	  perror("ERROR");
       }
    }
    int next = fork();
    if(! next){
      for(int i = 0
      execvp(line[0], line);
    }
  }
}

int main(){
  start();
}
