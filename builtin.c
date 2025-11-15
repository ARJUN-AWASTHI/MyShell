#include "shell.h"

int shell_exit(char **args){
   (void) args;
    p("Exiting shell...\n");
   exit(EXIT_SUCCESS);
}

int shell_env(char **args){
  extern char** environ;
  (void) args;
  if(!environ) return 1;
  for(int i = 0; environ[i]; i++){
    p("%s\n", environ[i]);
  }
  return 0;
}