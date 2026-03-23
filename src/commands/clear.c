#include <stdio.h>

int cmd_clear(char **argv, int argc){
	printf("\033[2J\033[H");
	fflush(stdout);
	return 0;
}
