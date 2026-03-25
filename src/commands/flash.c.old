#include <stdio.h>
#include "w25q128_driver.h"
#include <string.h>
#include <stdlib.h>
#include "shell.h"

static int readFlashAtAddress(uint32_t addr, uint32_t len){
	uint8_t buf[256]; 
    uint32_t bytes_left = len;
    uint32_t current_addr = addr;

    printf("\nADDR     | HEX DATA                                         | ASCII\n");
    printf("---------|--------------------------------------------------|--------\n");

    while (bytes_left > 0) {
        // decide how much to pull 256 bytes or whatever is left
        uint32_t chunk_size = (bytes_left > 256) ? 256 : bytes_left;

        // read from flash into 256-byte RAM chunk
        flash_read(current_addr, buf, chunk_size);

        // output
        for (uint32_t i = 0; i < chunk_size; i += 16) {
            printf("0x%06X | ", current_addr + i);

            //  Hex
            for (int j = 0; j < 16; j++) {
                if (i + j < chunk_size) printf("%02X ", buf[i + j]);
                else printf("   ");
            }

            printf(" | ");

            //  ASCII
            for (int j = 0; j < 16; j++) {
                if (i + j < chunk_size) {
                    uint8_t c = buf[i + j];
                    printf("%c", (c >= 32 && c <= 126) ? c : '.');
                }
            }
            printf("\n");
        }

        //  move the address forward, decrease remaining count
        current_addr += chunk_size;
        bytes_left -= chunk_size;

        //  safety check 
        if (current_addr >= 0x1000000) break;

        if(isUserInputAbort()){
        	printf("\n\nUser aborted. Exiting.\n\n");
        	return -1;
        } 
    }

    return 0;
}



static int readFlashID(){
	// read chip id 
	uint32_t id = flash_read_id();
	if (id == 0xEF4018) {
	    printf("Chip detected!\nID: 0x%06X\n", id);
	    return 0;
	} else {
	    printf("Error: Chip ID 0x%06X, No Chip detected!\n", id);
	    return -1;
	}
	
	
}

static int eraseFlashAtAddress(uint32_t addr){
	flash_write_enable();
	flash_erase_sector(addr);
	flash_wait_ready();
	printf("Done. Erasing at: %X\n", addr);
	return 0;
}

static int writeFlashAtAddress(uint32_t addr, const uint8_t *data, uint32_t len) {
    uint32_t bytes_left = len;
    uint32_t current_addr = addr;
    uint32_t data_offset = 0;

    while (bytes_left > 0) {
    
		// It finds how many bytes are left until the next 256-byte boundary.
        uint32_t max_this_page = 256 - (current_addr % 256);
        
        
        // take the smaller of: (Remaining Data) OR (Space left in Page)
        uint32_t chunk_size = (bytes_left < max_this_page) ? bytes_left : max_this_page;

        
        flash_write_enable();
        
        // write the chunk 
        flash_write_page(current_addr, &data[data_offset], chunk_size);
        
        flash_wait_ready(); 

        
        current_addr += chunk_size;
        data_offset += chunk_size;
        bytes_left -= chunk_size;
        
        printf("Progress: Written to 0x%06X (%u bytes left)\n", current_addr, bytes_left);
    }
    
    return 0;
}


int cmd_handleFlash(char **argv, int argc){
	// argv[0] is "flash"
	// argv[1] is the flag (like "-r")
		    
    if (argc < 2) {
        printf("Error: Missing arguments. Use -r to read, -i to get Chip ID\n");
        return -1;
    }

    // Handle READ: flash -r [addr] [len]
    if (strcmp(argv[1], "-r") == 0) {
        if (argc < 4) {
            printf("Usage: flash -r <addr> (0-FFFFFF) <len> (in bytes)\n");
            return -1;
        }
        // Convert strings to actual numbers
        uint32_t addr = strtoul(argv[2], NULL, 16); 
        if(addr > 0xffffff){
        	printf("Enter valid address! 0x000000 - 0xFFFFFF\n");
        	return -1;
        }
        uint32_t len  = strtoul(argv[3], NULL, 0);
        if(addr == 0xffffff && len > 16){
        	printf("tried to read out-of-bounds; setting byte length to 16bytes\n");
        	len = 16;
        }
        return readFlashAtAddress(addr,len);
        
    }
    else if (strcmp(argv[1], "-i") == 0){
    	return readFlashID();
    }
    else if(strcmp(argv[1], "-e") == 0){
    	// Convert strings to actual numbers
    	uint32_t addr = strtoul(argv[2], NULL, 16); 
    	if(addr > 0xffffff){
    		printf("Enter valid address! 0x000000 - 0xFFFFFF\n");
    		return -1;
    	}
    	printf("addr to erase: %x\n", addr);
    	return eraseFlashAtAddress(addr);
    }
    
	else if (strcmp(argv[1], "-w") == 0) {
    	if (argc < 4) {
        	printf("Usage: flash -w <addr_hex> <message>\n");
        	return 1;
    	}

    	uint32_t addr = strtoul(argv[2], NULL, 16);
    	char *text_to_write = argv[3]; // string
    
    	// calc length
    	uint32_t len = strlen(text_to_write); 

    	printf("Detected length: %u bytes\n", len);
    
    	return writeFlashAtAddress(addr, (uint8_t*)text_to_write, len);
    
	}
	else if(strcmp(argv[1], "--erase-all") == 0){
		printf("Erasing Flash. May take upto 30 seconds. DO NOT TURN OFF!\n");
		flash_write_enable();
		flash_erase_all();

		while(flash_is_busy()){
			printf(".");
			sleep_ms(1000);
		}
		
		printf("\nDone.\n");
		return 0;
	}
	else{
		printf("Error: Unknown arguments.\n");
		return -1;
	}

}
