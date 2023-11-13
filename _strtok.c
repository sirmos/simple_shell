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

										            
										            if (arg_count == MAX_ARGS - 1) {
												                    break;
														                }
											            }
				        }

		        args[arg_count] = NULL;
}

int main(void) {
	    char *line;
	        char *args[MAX_ARGS];

		    while (1) {
			            printf(":) ");
				            line = my_getline();

					            if (line == NULL) {
							                
							                printf("\n");
									            break;
										            }

						            if (strcmp(line, "exit") == 0) {
								                
								                free(line);
										            break;
											            }

							            
							            tokenize_line(line, args);

								            if (access(args[0], X_OK) == 0) {
										                
										                execute_command(args);
												        } else {
														            printf("Command not found: %s\n", args[0]);
															            }

									            free(line);
										        }

		        return 0;
}
