#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALIAS_COUNT 10
#define MAX_ALIAS_NAME_SIZE 20
#define MAX_ALIAS_VALUE_SIZE 50

typedef struct {
	    char name[MAX_ALIAS_NAME_SIZE];
	        char value[MAX_ALIAS_VALUE_SIZE];
} Alias;

Alias aliases[MAX_ALIAS_COUNT];
int alias_count = 0;

void print_aliases() {
	    for (int i = 0; i < alias_count; i++) {
		            printf("%s='%s'\n", aliases[i].name, aliases[i].value);
			        }
}

void print_alias(char *name) {
	    for (int i = 0; i < alias_count; i++) {
		            if (strcmp(aliases[i].name, name) == 0) {
				                printf("%s='%s'\n", aliases[i].name, aliases[i].value);
						            return;
							            }
			        }
}

void define_alias(char *name, char *value) {
	   
	    for (int i = 0; i < alias_count; i++) {
		            if (strcmp(aliases[i].name, name) == 0) {
				              
				                strncpy(aliases[i].value, value, sizeof(aliases[i].value));
						            return;
							            }
			        }

	       
	        if (alias_count < MAX_ALIAS_COUNT) {
			        strncpy(aliases[alias_count].name, name, sizeof(aliases[alias_count].name));
				        strncpy(aliases[alias_count].value, value, sizeof(aliases[alias_count].value));
					        alias_count++;
						    } else {
							            fprintf(stderr, "Error: Maximum number of aliases reached\n");
								        }
}

void process_alias_command(char *line) {
	    char *token = strtok(line, " ");
	        token = strtok(NULL, " ");

		    if (token == NULL) {
			            
			            print_aliases();
				        } else if (strchr(token, '=') != NULL) {
						        
						        while (token != NULL) {
								            char *name = strtok(token, "=");
									                char *value = strtok(NULL, "=");

											            if (name != NULL && value != NULL) {
													                    define_alias(name, value);
															                }

												                token = strtok(NULL, " ");
														        }
							    } else {
								            
								            while (token != NULL) {
										                print_alias(token);
												            token = strtok(NULL, " ");
													            }
									        }
}

int main(void) {
	    char *line = NULL;
	        size_t len = 0;

		    while (1) {
			            printf("$ ");
				            getline(&line, &len, stdin);

					           
					            line[strcspn(line, "\n")] = '\0';

						            if (strncmp(line, "exit", 4) == 0) {
								                
								                free(line);
										            exit(0);
											            } else if (strncmp(line, "alias", 5) == 0) {
													                
													                process_alias_command(line);
															        } else {
																	            
																	            printf("Command not supported. Try 'alias' or 'exit'.\n");
																		            }
							        }

		        return 0;
}
