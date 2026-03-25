#ifndef FS_H
#define FS_H

#include "lfs.h"
//access to filesystem 
int initFS();
extern lfs_t lfs;
extern struct lfs_config cfg;

#endif
