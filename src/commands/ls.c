#include <stdio.h>
#include "fs.h"
#include "envvars.h"
#include <string.h>
#include <stdbool.h>

int cmd_ls(char **argv, int argc){
	bool readAll = false;
	char * path = ENV_CURRENT_PATH;
	
	if (argc == 2) {
    // ls -a   OR   ls dir/
    	if (strcmp(argv[1], "-a") == 0) {
    	    readAll = true;
    	} 
    	else {
    	    path = argv[1];
    	}
	} 
	else if (argc >= 3) {
    // ls -a dir/   
    	if (strcmp(argv[1], "-a") == 0) {
        	readAll = true;
        	path = argv[2];
    	} 
	}


	lfs_dir_t dir;
	struct lfs_info info;

	int ret = lfs_dir_open(&lfs, &dir, path);
	if(ret < 0){
		printf("Error: Could not open list %s! (%d)\n",path,ret);
		return -1;
	}

	while(1){
		int res = lfs_dir_read(&lfs, &dir, &info);
		if(res < 0){
			printf("Error: Failed to read directory entry\n");
			break;
		}


		if (res == 0) break; //end of dir

		if(!readAll){
			if (strcmp(info.name, ".") == 0 || strcmp(info.name, "..") == 0) {
			    continue;
			}
		}

		if(info.type == LFS_TYPE_DIR){
			printf("%s/  ",info.name);
		}
		else{
			printf("%-10s\n",info.name);
		}
	}
	lfs_dir_close(&lfs, &dir);
	printf("\n");
	return 0;
}










void list_directory(const char *path) {
    lfs_dir_t dir;
    struct lfs_info info;

    // Open the directory
    int err = lfs_dir_open(&lfs, &dir, path);
    if (err < 0) {
        printf("Error: Could not open directory '%s' (Code: %d)\n", path, err);
        return;
    }

    printf("Listing directory: %s\n", path);
    printf("------------------------------------\n");

    // Loop through all files/folders
    while (true) {
        int res = lfs_dir_read(&lfs, &dir, &info);
        if (res < 0) {
            printf("Error: Failed to read directory entry\n");
            break;
        }
        
        if (res == 0) break; // End of directory (no more files)

        // Ignore the "." and ".." entries for a cleaner list
        if (strcmp(info.name, ".") == 0 || strcmp(info.name, "..") == 0) {
            continue;
        }

        // Distinguish between Folder and File
        if (info.type == LFS_TYPE_DIR) {
            printf("  [DIR]  %s\n", info.name);
        } else {
            printf("  [FILE] %-20s  %ld bytes\n", info.name, info.size);
        }
    }

    lfs_dir_close(&lfs, &dir);
    printf("------------------------------------\n");
}
