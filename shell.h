#ifndef SHELL_H
#define SHELL_H

#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define p(...) printf(__VA_ARGS__)
#define DEL " \t\r\n\a\v\f\r"
#define HOME "/mnt/d"

// BuiltIn functions(builtin.c)
typedef struct buitlin {
  char* name;
  int (*func)(char**);
} t_buitlin;

int shell_exit(char**);
int shell_env(char**);
int shell_clear(char**);
int shell_cd(char** );

// Utility Function (utils.c)
void printBanner();
char* dir_generator(const char* , int );
char** my_completion(const char* , int , int );



#endif