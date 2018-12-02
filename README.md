gash
by Angela Li and George Liang

Features:
  Forks and executes commands
  Reads and separates multiple commands on one line


Attempted:


Bugs:
  Exit occasionally does not work

Files & Function Headers:
  gash.c
    char ** parse_args()
      Inputs: char * line
      Returns: Array of strings where each entry is a token

    void cd()
      Inputs:
      Returns:

    void redirect_output(char ** in, char *** ln, int * fd)
      Inputs: input line, container for parsed input, file descriptor to write to
      Returns:

    void redirect_input(char **in, char *** ln, int * fd)
      Inputs: input line, container for parsed input, file descriptor to read from
      Returns:
      
    void redirect_compound(char **in, char ***ln, int * fdW, int * fdR)
      Inputs: input line, container for parsed input, file descriptor to write to, file descriptor to read from
      Returns:

    void start()
      Inputs:
      Returns:
