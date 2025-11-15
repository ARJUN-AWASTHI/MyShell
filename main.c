#include "shell.h"
t_buitlin global_buitlin[] ={
		//{.name = "cd", .func =shell_cd},
		//{.name = "env", .func =shell_env},
		{.name = "exit", .func =shell_exit},
		{.name = NULL}
};


int status = 0;
		
	




void shell_exec(char **args){
	const char* curr;
	int i = 0;
	while ((curr = global_buitlin[i].name)){
		if(!strcmp(curr, args[0])){
			status = global_buitlin[i].func(args);
			return;
		}
		i++;
	}

	//shell_launch(args);
}

char **shell_split_line(char *line){
	char **tokens = NULL;
	int i  = 0;
	tokens = malloc(sizeof(char *) * 1024);
	
	for(char* token = strtok(line, DEL); token; token = strtok(NULL, " \t\r\n\a")) {
		tokens[i] = token;
		i++;
	}
	tokens[i] = NULL;
	return tokens;
}

char *shell_read_line(void) {
	char *buff = NULL;
	size_t buffsize = 0;
	char cwd[1024];

	if (getcwd(cwd, sizeof cwd) == NULL) {
		perror("getcwd() error");
	}

	p("%s $>", cwd);
	if (getline(&buff, &buffsize, stdin) == -1) {
		free(buff);
		buff = NULL;
		if (feof(stdin)) {
			p("[EOF]");

		} else {
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}

	return buff;
}

int main(int ac, char **av) {
	char *line;
	char **args;
	printBanner();
	while ((line = shell_read_line())) {

		args = shell_split_line(line);
		shell_exec(args);
		
		


		free(line);
		free(args);
	}

	return EXIT_SUCCESS;
}