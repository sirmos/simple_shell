#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_SIZE 100
#define MAX_ARGS 10

void execute_command(char *args[]) {
	    pid_t pid = fork();

	        if (pid == -1) {
			        perror("fork");
				        exit(EXIT_FAILURE);
					    } else if (pid == 0) {
						            
						            execvp(args[0], args);
							           
							            perror("exec");
								            exit(EXIT_FAILURE);
									        } else {
											        
											        int status;
												        waitpid(pid, &status, 0);
													        if (WIFEXITED(status)) {
															            exit(WEXITSTATUS(status));
																            }
														    }
}

void tokenize_line(char *line, char *args[]) {
	    int arg_count = 0;
	        char *token = strtok(line, " \t\n");

		    while (token != NULL) {
			            args[arg_count++] = token;
				            token = strtok(NULL, " \t\n");

					            if (arg_count == MAX_ARGS - 1) {
							                break;
									        }
						        }

		        args[arg_count] = NULL;
}

int main(int argc, char *argv[]) {
	    if (argc != 2) {
		            fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
			            exit(EXIT_FAILURE);
				        }

	        FILE *file = fopen(argv[1], "r");
		    if (file == NULL) {
			            perror("fopen");
				            exit(EXIT_FAILURE);
					        }

		        char line[MAX_COMMAND_SIZE];
			    while (fgets(line, sizeof(line), file) != NULL) {
				            
				            line[strcspn(line, "\n")] = '\0';

					            
					            char *args[MAX_ARGS];
						            tokenize_line(line, args);
							            if (args[0] != NULL) {
									                execute_command(args);
											        }
								        }

			        fclose(file);

				    return 0;
}
