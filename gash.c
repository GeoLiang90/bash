#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>

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
    //NOTE: This puts the working directory into w_dir
    char * w_dir = malloc(100);
    //NOTE: The ~ directory is skipped. It is short for home which is already present
    printf("%s:$ ", getcwd(w_dir,100));

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
    //NOTE: This wait here should fix the problem with ls not working properly
    int status;
    //Status just allows me to give wait an argument
    wait(&status);

    if(! next){
      execvp(line[0], line);
    }
  }
}

int main(){
  start();
}
