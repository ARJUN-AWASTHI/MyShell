#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h> 


#define p(...) printf(__VA_ARGS__)
#define DEL " \t\r\n\a\v\f\r"

typedef struct buitlin{
  char *name;
  int (*func)(char **);
}t_buitlin;

int shell_exit(char **); 
int shell_env(char **); 


void printBanner();
#endif