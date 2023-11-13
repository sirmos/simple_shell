#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_COMMAND_SIZE 100
#define MAX_ARGS 10

char buffer[MAX_BUFFER_SIZE];
size_t buffer_pos = 0;
size_t buffer_size = 0;

char* my_getline(void) {
	    if (buffer_pos >= buffer_size) {
		            
		            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
			            if (bytes_read <= 0) {
					                
					                return NULL;
							        }
				            buffer_pos = 0;
					            buffer_size = (size_t)bytes_read;
						        }

	        
	        size_t start = buffer_pos;
		    while (buffer_pos < buffer_size && buffer[buffer_pos] != '\n') {
			            buffer_pos++;
				        }

		        
		        size_t length = buffer_pos - start;
			    char *line = malloc(length + 1);
			        if (line == NULL) {
					        perror("malloc");
						        exit(EXIT_FAILURE);
							    }
				    strncpy(line, buffer + start, length);
				        line[length] = '\0';

					   
					    if (buffer_pos < buffer_size && buffer[buffer_pos] == '\n') {
						            buffer_pos++;
							        }

					        return line;
}

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
													        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
															            exit(WEXITSTATUS(status));
																            }
														    }
}

void tokenize_line(char *line, char *args[]) {
	    int arg_count = 0;
	        char *token = line;

		    
		    for (size_t i = 0; i < strlen(line); i++) {
			            if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
					                
					                line[i] = '\0';

							            
							            if (i > 0 && line[i - 1] != '\0') {
									                    args[arg_count++] = token;
											                }

								               
								                token = line + i + 1;

										        
										          **  if (arg_count == MAX_ARGS - 1) {
												                  break;
														              }
											          }
				        }

		        args[arg_count] = NULL;
}

void execute_logical_operator(char *line) {
	    char *commands[MAX_ARGS];
	        int command_count = 0;
		    char *token = strtok(line, "&&||");

		        while (token != NULL) {
				        commands[command_count++] = token;
					        token = strtok(NULL, "&&||");
						    }

			    int i = 0;
			        while (i < command_count) {
					        char *args[MAX_ARGS];
						        tokenize_line(commands[i], args);

							        if (args[0] != NULL) {
									            if (strcmp(args[0], "exit") == 0) {
											                    
											                    int status = (args[1] != NULL) ? atoi(args[1]) : 0;
													                    exit(status);
															                } else {
																		               
																		                execute_command(args);

																				               
																				                if (i + 1 < command_count) {
																							                    if (strcmp(commands[i + 1], "&&") == 0 && WEXITSTATUS(status) == 0) {
																										                            i += 2;
																													                        } else if (strcmp(commands[i + 1], "||") == 0 && WEXITSTATUS(status) != 0) {
																																	                        i += 2;
																																				                    }
																									                    }
																						            }
										            }
								    }
}

int main(void) {
	    char *line;

	        while (1) {
			        char current_directory[PATH_MAX];
				        getcwd(current_directory, sizeof(current_directory));
					        printf("%s$ ", current_directory);
						        line = my_getline();

							        if (line == NULL) {
									            
									            printf("\n");
										                break;
												        }

								        
								        execute_logical_operator(line);

									        free(line);
										    }

		    return 0;
}
