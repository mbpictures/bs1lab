//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//
#include <stdint.h>
#include <sys/stat.h>

#ifndef myfs_structs_h
#define myfs_structs_h

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

#define DATA_BLOCKS 65536

#define NULL nullptr

#define SUPERBLOCK_START_BLOCK 0
#define ROOT_START_BLOCK 273
#define DATA_START_BLOCK 4881

typedef struct {
	char filename[8];
	uint8_t uid;
	uint8_t gid;
	uint32_t sizeOfFile;
	mode_t mode;
	uint32_t ctime;
	uint32_t atime;
	uint32_t mtime;
	uint16_t firstBlock;
}FileEntry;

// TODO: Add structures of your file system here

#endif /* myfs_structs_h */
