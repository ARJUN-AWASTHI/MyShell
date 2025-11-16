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

void shell_exec_single(char** args) {
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

// ADD Functionality of pipes
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

void shell_exec_pipe(char* line) {
  char** cmds = NULL;
  int i = 0;

  int cmd_count = 0;
  cmds = malloc(sizeof(char*) * 1024);
  char* token = strtok(line, "|");
  while (token) {
    cmds[i] = token;
    token = strtok(NULL, "|");
    cmd_count++;
    i++;
  }

  int pipefd[cmd_count - 1][2];

  // Make pipes
  for (int i = 0; i < cmd_count - 1; i++) {
    pipe(pipefd[i]);
  }

  for (int i = 0; i < cmd_count; i++) {
    pid_t pid = fork();

    if (pid == 0) {
      // If not first command → redirect stdin
      if (i > 0) {
        dup2(pipefd[i - 1][0], STDIN_FILENO);
      }

      // If not last command → redirect stdout
      if (i < cmd_count - 1) {
        dup2(pipefd[i][1], STDOUT_FILENO);
      }

      // Close all pipes in the child
      for (int j = 0; j < cmd_count - 1; j++) {
        close(pipefd[j][0]);
        close(pipefd[j][1]);
      }

      
      char** args = shell_split_line(cmds[i]);
      execvp(args[0], args);

      perror("execvp failed");
      exit(1);
    }
  }

  // Parent closes all pipes
  for (int i = 0; i < cmd_count - 1; i++) {
    close(pipefd[i][0]);
    close(pipefd[i][1]);
  }

  // Parent waits
  for (int i = 0; i < cmd_count; i++) {
    wait(NULL);
  }
}

void execute(char* line) {
  int len = strlen(line);
  for (int i = 0; i < len; i++) {
    if (line[i] == '|') {
      shell_exec_pipe(line);
      return;
    }
  }
  char** args;
  args = shell_split_line(line);
  shell_exec_single(args);

  free(args);
  return;
}

int main(int ac, char** av) {
  char* line;

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

    execute(line);
    // args = shell_split_line(line);
    // shell_exec_single(args);

    free(line);
  }
  printf("\033[H\033[J");
  return EXIT_SUCCESS;
}