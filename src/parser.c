#include "pico/stdlib.h"
#include <stdio.h>
#include "def.h"
#include "parser.h"


#include <string.h> 

int parseBuffer(char *lineBuffer, char **argv, int *argc) {
    int argCount = 0;

    // Get the first token
    char *token = strtok(lineBuffer, " ");

    while (token != NULL && argCount < MAX_ARGS) {
        argv[argCount] = token; // store the pointer to the start of the word
        argCount++;
        
        // calls to strtok must use NULL
        token = strtok(NULL, " ");
    }

    *argc = argCount; 
    return 0;
}


