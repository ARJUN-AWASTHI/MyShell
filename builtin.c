#include "shell.h"

int shell_exit(char** args) {
  (void)args;
  p("Exiting shell...\n");
  printf("\033[H\033[J");
  exit(EXIT_SUCCESS);
}

int shell_cd(char** args) {
  if (args[1] == NULL) {
    
    //char* home = getenv("HOME");
    char* home = HOME;
    if (home == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }
    if (chdir(home) != 0) perror("cd");
    return 1;
  }

  if (chdir(args[1]) != 0) {
    perror("cd");
  }

  return 1;
}

int shell_env(char** args) {
  extern char** environ;
  (void)args;
  if (!environ) return 1;
  for (int i = 0; environ[i]; i++) {
    p("%s\n", environ[i]);
  }
  return 0;
}

int shell_clear(char** args) {
  (void)args;
  printf("\033[H\033[J");
  printBanner();
  return 0;
}