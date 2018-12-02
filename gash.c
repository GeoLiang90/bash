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
    /*
    if(strstr(" ", temp)){
      printf("Space found");
      temp = strsep(&temp, " ");
    }
    */
    //printf("%s+%s \n", temp, "x");
    //char ch = temp[0];
    //while()
    if(strcmp(temp,"\0") != 0){
      arr[i] = temp;
    i++;
      }
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

void redirect_output(char ** in, char *** ln, int * fd){
  //User typed line, Array to hold parsed command, File descriptor to output to
  char * holder;
  char ** file;
  //I want the first occurence to be executed
  holder = strsep(in, ">");
  //printf("%s\n",holder);
  *ln = parse_args(holder);
  file = parse_args(*in);
  //printf("%s\n", input);
  //printf("%s\n",file[0]);
  *fd = open(file[0], O_CREAT | O_WRONLY, 0666);
  if (*fd < 0){
    perror("ERROR");
  }
}

void redirect_input(char **in, char *** ln, int * fd){
  char* holder;
  char ** file;

  holder = strsep(in, "<");

  *ln = parse_args(holder);

  file = parse_args(*in);

  *fd = open(file[0], O_RDONLY);

  if (*fd < 0){
    perror("ERROR");
  }

}

void redirect_compound(char **in, char ***ln, int * fdW, int * fdR){
  //Compound assumes that you want to run command < file then put that into a file

  char * holder;
  //UP just means unprocessed
  char ** wFile;
  char ** rFile;

  //Take care of the command
  holder = strsep(in, "<");
  *ln = parse_args(holder);

  rFile = parse_args(strsep(in, ">"));
  //rFile = parse_args(rFileUP)
  wFile = parse_args(*in);
  /*
  printf("%s \n", rFile[0]);
  printf("%s \n", wFile[0]);
  */
  *fdW = open(wFile[0], O_CREAT | O_WRONLY, 0666);
  *fdR = open(rFile[0], O_RDONLY);
  if (*fdW < 0){
    perror("ERROR");
  }
  if (*fdR < 0){
    perror("ERROR");
  }

  /*
  files[0] = parse_args(strsep(in, ">"));
  files[1] = parse_args(*in);
  int i = 0;
  while(files[i]){
    printf("%s\n", files[i]);
  }
  */
}
void piper(char **in, char *** ln, char*** commandTwo, int ** pipeArray){
  char * holder;
  holder = strsep(in, "|");
  //Gives it the first command
  *ln = parse_args(holder);
  //printf("First Command: %s\n ",*commandOne[0]);
  *commandTwo = parse_args(*in);
  //printf("Second Command: %s\n ",*commandTwo[0]);

 pipe(*pipeArray);
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
      int toWrite = 0;
      int toRead = 0;
      int * pipes = calloc(sizeof(int),2);
      char * r_out;
      char * r_in;
      char * p_ipe;
      char ** cmdTwo = calloc(sizeof(char*), 10);

      //If there is more than one command
      if(multi > 1){
        //Parse multiple times each argument in arr to put into input
        //Check for redirect in arr[x]
        r_out = strstr(arr[x],">");
        r_in = strstr(arr[x],"<");
        //p_ipe = strstr(arr[x], "|");
        if (r_out && r_in){
          //In the special case that both are there
          //printf("Running compound \n");
          redirect_compound(&arr[x],&line,&toWrite,&toRead);
        }
        else if(r_out){
          redirect_output(&arr[x],&line,&toWrite);
        }

        else if(r_in){
          redirect_input(&arr[x],&line,&toRead);
        }

        else{
          line = parse_args(arr[x]);
        }
        x += 1;
        input = arr[x];
      }
      else{
        //Check for a redirect
        r_out = strstr(input,">");
        r_in = strstr(input,"<");
        p_ipe = strstr(input,"|");
        //printf("%s\n", p_ipe);

        if(p_ipe){
          //line = parse_args(input);
          piper(&input,&line,&cmdTwo,&pipes);
        }

        else if (r_out && r_in){
        //if(r_out && r_in){
          //In the special case that both are there
          //printf("Running compound \n");
          redirect_compound(&input,&line,&toWrite,&toRead);
        }
        else if(r_out){
          redirect_output(&input,&line,&toWrite);
        }

        else if(r_in){
          redirect_input(&input,&line,&toRead);
        }
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


      if(!next){

        if(toWrite){
          dup2(toWrite,STDOUT_FILENO);
        }

        if (toRead){
          dup2(toRead,STDIN_FILENO);
        }
        /*
        if (p_ipe){
          printf(" I running");
          //Everything from STDOUT should be written to this pipe
          dup2(pipes[WRITE],STDOUT_FILENO);
          //Everything from STDIN should be read from this pipe
          //dup2(pipes[WRITE],STDIN_FILENO);
          //ls would be run an outputted to pipes[WRITE]
          execvp(cmdOne[0], cmdOne);
          //wc will now check for STDIN and should be handed pipes[WRITE]
          //execvp(cmdTwo[0], cmdTwo);
        }
        */
        if (p_ipe){
          int c1 = fork();
          if (!c1){
          dup2(pipes[WRITE],STDOUT_FILENO);
          execvp(line[0],line);
          }
          //dup2(pipes[WRITE],STDIN_FILENO);
        }
        else{
        execvp(line[0], line);
        }
        if (p_ipe){
          /*
          char ** f = calloc(sizeof(char*),1);
          f[0] = "/bin/stdout";
          */
          execvp(cmdTwo[0], f);
        }
        //}

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

        if(toWrite){
          int clW;
          clW = close(toWrite);
          toWrite = 0;
          if (clW < 0){
            perror("ERROR CLOSING toWrite");
          }
          /*
          else{
            printf("Successfully closed toWrite\n");
            printf("toWrite: %d\n", toWrite);
          }
          */
        }
        if(toRead){
          int clR;
          clR = close(toRead);
          toRead = 0;
          if (clR < 0){
            perror("ERROR CLOSING toRead");
          }
        }

        if(pipes[READ]){
          close(pipes[READ]);
        }
        if(pipes[WRITE]){
          close(pipes[WRITE]);
        }
          /*
          else{
            printf("Successfully closed toRead\n");
          }
          */



      multi -= 1;
    } //multi while loop ends here
  }
}

int main(){
  start();
}
