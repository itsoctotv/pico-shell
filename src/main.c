#include "pico/stdlib.h"
#include <stdio.h>
#include "shell.h"

int main(){

	
	stdio_init_all();
	shellLoop();
	return 0;
}
