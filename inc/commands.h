#ifndef COMMANDS_H
#define COMMANDS_H

typedef int (*commandHandlerFunc)(char **argv, int argc);

typedef struct{
	char *name;
	commandHandlerFunc func;
}Command;


#endif
