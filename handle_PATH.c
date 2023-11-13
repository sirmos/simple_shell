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
													    }
}

int main(void) {
	    char command[MAX_COMMAND_SIZE];
	        char *args[MAX_ARGS];

		    while (1) {
			            printf(":) ");
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

											            if (access(args[0], X_OK) == 0) {
													               
													                execute_command(args);
															        } else {
																	            
																	            char *path = getenv("PATH");
																		                char *path_copy = strdup(path);
																				            char *path_token = strtok(path_copy, ":");

																					                while (path_token != NULL) {
																								                char full_path[MAX_COMMAND_SIZE];
																										                snprintf(full_path, sizeof(full_path), "%s/%s", path_token, args[0]);

																												                if (access(full_path, X_OK) == 0) {
																															                    
																															                    args[0] = full_path;
																																	                        execute_command(args);
																																				                    break;
																																						                    }

																														                path_token = strtok(NULL, ":");
																																            }

																							            free(path_copy);
																								            }
												        }

		        return 0;
}

