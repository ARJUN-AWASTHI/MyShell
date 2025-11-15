#include "shell.h"
t_buitlin global_buitlin[] = {{.name = "clear", .func = shell_clear},
                              {.name = "env", .func = shell_env},
                              {.name = "exit", .func = shell_exit},
                              {.name = "cd", .func = shell_cd},
                              {.name = NULL}};

int status = 0;

void shell_launch(char** args) {
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    perror("Forking failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("Failed to execute Command");
      exit(EXIT_FAILURE);
    }
  }

  else
    wait(&status);
}

void shell_exec(char** args) {
  const char* curr;
  int i = 0;
  while ((curr = global_buitlin[i].name)) {
    if (!strcmp(curr, args[0])) {
      status = global_buitlin[i].func(args);
      return;
    }
    i++;
  }

  shell_launch(args);
}
//ADD Functionality of pipes 
char** shell_split_line(char* line) {
  char** tokens = NULL;
  int i = 0;
  tokens = malloc(sizeof(char*) * 1024);

  for (char* token = strtok(line, DEL); token;
       token = strtok(NULL, " \t\r\n\a")) {
    tokens[i] = token;
    i++;
  }
  tokens[i] = NULL;
  return tokens;
}

int main(int ac, char** av) {
  char* line;
  char** args;
  rl_bind_key('\t', rl_complete);
  rl_attempted_completion_function = my_completion;

  printf("\033[H\033[J");
  printBanner();

  // char* home = getenv("HOME");
  char* home = HOME;
  chdir(home);
  while (1) {
    char cwd[1024];

    if (getcwd(cwd, sizeof cwd) == NULL) {
      perror("getcwd() error");
    }

    char prompt[1060];  // enough room for cwd + "$ >"
    snprintf(prompt, sizeof(prompt), "%s $ > ", cwd);

    line = readline(prompt);
    if (!line)
      break;
    else if (strcmp(line, "") == 0)
      continue;
    if (*line) add_history(line);

    args = shell_split_line(line);
    shell_exec(args);

    free(line);
    free(args);
  }
  printf("\033[H\033[J");
  return EXIT_SUCCESS;
}