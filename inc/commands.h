#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdio.h>

typedef int (*commandHandlerFunc)(char **argv, int argc);

typedef struct{
	char *name;
	commandHandlerFunc func;
}Command;

// declare external functions so the compiler sees them
extern int cmd_help(char **argv, int argc);
extern int cmd_clear(char **argv, int argc);
//filesystem commands
extern int cmd_ls(char **argv, int argc);
extern int cmd_cd(char **argv, int argc);

Command cmd_table[] = {
    {"help",   cmd_help},
    {"clear",   cmd_clear},
    {"ls",   cmd_ls},
    {"cd",   cmd_cd},
    {NULL,   NULL} 
};



#endif
