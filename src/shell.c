#include "pico/stdlib.h"
#include <stdio.h>
#include "def.h"
#include "bufferHandling.h"
#include "shell.h"
#include <string.h>
#include "parser.h"


char lineBuffer[MAX_BUFFER_LENGTH];
char *argv[MAX_ARGS];
int argc;

int shellLoop(){

	while (1){
		printf("> ");
		
		// clear buffers
		memset(lineBuffer, '\0', MAX_BUFFER_LENGTH);
		memset(argv, '\0', MAX_ARGS);
		argc = 0x00;
		
		//read,parse,exec

		//read
		int len = readUARTLine(lineBuffer);

		//parse
		if(len > 0){
			//buff filled with args
			int ret = parseBuffer(lineBuffer, argv, &argc);
		}

		//exec
				
		//debug print args
		for(int i = 0; i < argc;i++){
			printf("%s \t %d %d\n",argv[i], i, argc);
		}
		
	}
	return 0;
}
