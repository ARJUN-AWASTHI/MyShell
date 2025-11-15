#include "shell.h"

int shell_exit(char **args){
   (void) args;
    p("Exiting shell...\n");
   exit(EXIT_SUCCESS);
}