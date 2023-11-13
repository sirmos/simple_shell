#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_SIZE 100
#define MAX_ARGS 10

int main(void) {
	    char command[MAX_COMMAND_SIZE];
	        char *args[MAX_ARGS];

		    while (1) {
			            printf("Enter command with arguments (or type 'exit' to quit): ");
				            if (fgets(command, sizeof(command), stdin) == NULL) {
						                
						                printf("\n");
								            break;
									            }

					            
					            command[strlen(command) - 1] = '\0';

						            if (strcmp(command, "exit") == 0) {
								                
								                break;
										        }

							            
							            int arg_count = 0;
								            char *token = strtok(command, " ");
									            while (token != NULL && arg_count < MAX_ARGS - 1) {
											                args[arg_count++] = token;
													            token = strtok(NULL, " ");
														            }
										            args[arg_count] = NULL;

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
																													        }
													        }

		        return 0;
}
