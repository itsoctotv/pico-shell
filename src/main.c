#include "pico/stdlib.h"
#include <stdio.h>
#include "shell.h"
#include "w25q128_driver.h"
int main(){

	
	stdio_init_all();
/*	flash_init();
	sleep_ms(2000);
	// Verify Chip
	uint32_t id = flash_read_id();
	if (id == 0xEF4018) {
	    printf("Hard Drive Detected: Winbond 16MB\n");
	} else {
	    printf("Error: Chip ID 0x%06X (Check wiring!)\n", id);
	}

	// Example: Read some of that Intel BIOS you saw earlier
	uint8_t buffer[64];
	flash_read(0xFFF6E0, buffer, 64);
	printf("Intel Residual Data: %s\n", buffer);
	sleep_ms(2000);*/
	shellLoop();
	return 0;
}
