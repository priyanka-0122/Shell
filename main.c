#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_SIZE	100

void init_shell() {
	// set colour to terminal path
	printf("*********************************\n");
	printf("******Welcome to User Shell******\n");
	printf("*********************************\n");
}

void* takeInput(char *string) {
	fgets(string, MAX_SIZE, stdin);
}

void printDir() {
	char cwd[MAX_SIZE];

	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s$ ", cwd);
	} else {
		perror("getcwd() error");
	}
}

int checkCommand(char *string) {
	char *args[MAX_SIZE];   // argument list
	int i = 0;
	int status, pid;

	// remove newline
	// Find the number of characters in "string" before the 1st occurrence of new line
	string[strcspn(string, "\n")] = '\0';

	// tokenize input
	args[i] = strtok(string, " ");
	while (args[i] != NULL && i < (MAX_SIZE-1)) {
		i++;
		args[i] = strtok(NULL, " ");
	}

	if (args[0] == NULL)
		return 1; // empty input

	pid = fork();

	if (pid < 0) {
		perror("fork failed");
		return 0;
	} else if (pid == 0) {
		// child process
		execvp(args[0], args);

		// if exec fails
		perror("exec failed");
		return 0;
	} else {
		// parent process
		waitpid(pid, &status, 0);
	}
	return 1;
}

int main() {
	char string[MAX_SIZE];
	init_shell();
	while(1) {
		printDir();
		if (takeInput(string) == NULL) {
			printf("%s\n", string);
			printf("Failed to accept command\n");
		}
		if (!checkCommand(string)) {
			printf("ERROR: Command %s not found\n", string);		
			printf("Please type the correct command\n");
		}
	}
}
