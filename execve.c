#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_SIZE 100

int main(void)
{
	char command[MAX_COMMAND_SIZE];

	while (1)
       	{
		printf("simple_shell$ ");
		if (fgets(command, sizeof(command), stdin) == NULL)
	       	{

			printf("\n");
			break;
		}

		command[strlen(command) - 1] = '\0';

		pid_t pid = fork();

		if (pid == -1)
	       	{
			perror("fork");
			exit(EXIT_FAILURE);
		}
	       	else if (pid == 0)
	       	{
			execlp(command, command, (char *)NULL);
			perror("exec");
			exit(EXIT_FAILURE);
		}
	       	else
	       	{
			int status;
			waitpid(pid, &status, 0);
		}
	}

	return 0;
}
