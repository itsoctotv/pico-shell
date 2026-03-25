#include "pico/stdlib.h"
#include <stdio.h>
#include "shell.h"
#include "fs.h"

//#include "w25q128_driver.h"
int main(){

	
	stdio_init_all();
	//flash_init();
	sleep_ms(2000);
	printf("\033[32mBoot up... (delaying start by 3000ms)\033[0m\n");
	sleep_ms(3000);
	printf("\033[32mInit FS...\033[0m\n");
	initFS();	
	printf("\033[32mStarting Shell...\033[0m\n\n");
	shellLoop();
	return 0;
}
