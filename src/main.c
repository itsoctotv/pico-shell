#include "pico/stdlib.h"
#include <stdio.h>
#include "shell.h"
#include "w25q128_driver.h"
int main(){

	
	stdio_init_all();
	flash_init();
	
	shellLoop();
	return 0;
}
