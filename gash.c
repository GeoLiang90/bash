#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

char ** parse_args(char *line){
  char ** arr = calloc(sizeof(char*), 5);
  char * temp;
  int i = 0;
  while (line) {
    temp = strsep(&line, " ");
    if(strstr(" ", temp)){
      temp = strsep(&temp, " ");
    }
    //printf("%s+%s \n", temp, "x");
    if(temp)
      arr[i] = temp;
    i++;
  }
  return arr;
}

char ** parse_arr(char * line){
  char ** arr = calloc(sizeof(char*), 5);

}

void cd(char * dir){
  //printf("%s \n", dir);
  int z = chdir(dir);
  if (z < 0){
    perror("ERROR");
   }
}

void start(){
  while(1){
    //printf("%s: ", getcwd());
    char * input = malloc(100);
    fgets(input,100,stdin);
    input[strlen(input)-1] = 0;
    char ** line = parse_args(input);
    //printf("%s \n", commands[0]);
    //char bin[50] = "/bin/";
    //char * path = strcat(bin, commands[0]);
    //printf("%s \n", path);
    if (strcmp(line[0],"cd") == 0){
	     cd(line[1]);
     }
    int next = fork();
    if(! next){
      execvp(line[0], line);
    }
  }
}

int main(){
  start();
}
