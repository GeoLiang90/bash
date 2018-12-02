#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>
#define READ 0
#define WRITE 1
char ** parse_args(char *line){
  char ** arr = calloc(sizeof(char*), 5);
  char * temp;
  int i = 0;
  while (line) {
    temp = strsep(&line, " ");
    //printf("%s", temp);
    char ch = temp[0];
    /*
    if(strstr(" ", temp)){
      printf("Space found");
      temp = strsep(&temp, " ");
    }
    */
    //printf("%s+%s \n", temp, "x");
    //char ch = temp[0];
    //while()
    if(temp && (strcmp(temp,"\0") != 0))
      arr[i] = temp;
    i++;
  }
  return arr;
}

void cd(char * dir){
  //printf("%s \n", dir);
  int z = chdir(dir);
  if (z < 0){
    perror("ERROR");
   }
}

void redirect_output(char * input){
  char * holder;
  //int fd = open(file, O_WRONLY, 0777);
}

void redirect_input(){

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

    int multi = 1;
    char ** arr = calloc(sizeof(char*), 10);
    //If input has a semicolon
    if(strstr(input,";")){
      //printf("has ; \n");
      char * temp;
      int i = 0;
      multi = 0;
      //Separate our input by semicolon and add to multi
      while(input){
        temp = strsep(&input, ";");
        if(temp){
            arr[i] = temp;
            multi += 1;
        }
        i++;
        //printf("%d \n", multi);
      }
    }

    // if(strstr(input,">")){ //|| strstr(input,"<")){
    //   char * temp;
    //   char * og = strsep(&input,">");
    //   temp = strsep(&input,">");
    //   printf("%s \n", temp);
    //   redirect(temp);
    //   input = og;
    // }
    //x keeps track of index
    int x = 0;
    while(multi){
      //printf("%d \n", multi);
      char ** line = calloc(sizeof(char*), 10);
      int toWrite;
      int toRead;
      //If there is more than one command
      if(multi > 1){
        //Parse multiple times each argument in arr to put into input
        line = parse_args(arr[x]);
        x += 1;
        input = arr[x];
      }
      else{
        //Check for a redirect

        char * holder;
        char * r_in = strstr(input,">");
        if(r_in){

          //I want the first occurence to be executed
          holder = strsep(&input, ">");
          //printf("%s\n",holder);
          line = parse_args(holder);

          //printf("%s\n", input);
          toWrite = open(input, O_CREAT | O_WRONLY, 0666);
        //  lseek(toWrite,0,SEEK_SET);
          //dup2(toWrite,STDOUT_FILENO);
          //toRead = open("stdout.txt",O_RDONLY);
        }
        /*
        if(strstr(input, "<")){

        }
        */
        else{
          line = parse_args(input);
        }
      }
      //printf("%s \n", commands[0]);
      //char bin[50] = "/bin/";
      //char * path = strcat(bin, commands[0]);
      //printf("%s \n", path);
      if (strcmp(line[0],"cd") == 0){
  	     cd(line[1]);

       }

       if (strcmp(line[0],"exit")
        == 0){
  	     exit(0);

      }

      int next = fork();
      //NOTE: This wait here should fix the problem with ls not working properly
      int status;
      //Status just allows me to give wait an argument
      wait(&status);

      if(! next){
        //Getting caught up here
        if(toWrite){
          dup2(toWrite,STDOUT_FILENO);
        }
        execvp(line[0], line);
        close(toWrite);
        /*
        if(toWrite){
          printf("Works");
          close(toWrite);

        }
        /*
        if(toWrite){
          char * out = malloc(100);
          while(fgets(out,sizeof(out),stdout)){
            lseek(toWrite,0,SEEK_END);
            write(toWrite,out,sizeof(out));
          }
          */
        }
        close(toWrite);
      multi -= 1;
    } //multi while loop ends here
  }
}

int main(){
  start();
}
