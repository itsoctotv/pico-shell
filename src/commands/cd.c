#include "envvars.h"
#include "fs.h"
#include <string.h>
#include <stdio.h>

int cmd_cd(char **argv, int argc) {
    struct lfs_info info;
    char temp_path[128];
    char *path_to_change = "/";

    if (argc >= 2) {
        path_to_change = argv[1];
    }

    // test path
    if (path_to_change[0] == '/') {
        // absolute path
        snprintf(temp_path, sizeof(temp_path), "%s", path_to_change);
    } 
    else {
        // relative path, combine with current directory
        // if current dir is root dont add /
        if (strcmp(ENV_CURRENT_PATH, "/") == 0) {
            snprintf(temp_path, sizeof(temp_path), "/%s", path_to_change);
        } 
        else {
            snprintf(temp_path, sizeof(temp_path), "%s/%s", 
            	ENV_CURRENT_PATH, path_to_change);
        }
    }

    int err = lfs_stat(&lfs, temp_path, &info);

    if (err < 0) {
        printf("cd: %s: No such file or directory\n", path_to_change);
        return -1;
    }

    // actually a directory
    if (info.type != LFS_TYPE_DIR) {
        printf("cd: %s: Not a directory\n", path_to_change);
        return -1;
    }

    strncpy(ENV_CURRENT_PATH, temp_path, 128);
    
    return 0;
}
