#include "pico/stdlib.h"
#include <stdio.h>
#include "def.h"
#include "executer.h"
#include <string.h> 

#include "commands.h"




int executeCommand(char **argv, int argc){
	if(argc <= 0 || argv[0] == NULL){
		return 1; // no commands in argv
	}
	
	//look up command

	for(int i = 0; cmd_table[i].name != NULL; i++){
		if(strcmp(argv[0], cmd_table[i].name) == 0){
			return cmd_table[i].func(argv, argc); // call the function
			
		}
	}
	printf("Unknown command '%s'.\n",argv[0]);
	return -1;
}
