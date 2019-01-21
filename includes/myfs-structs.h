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

#define NAME_LENGTH 259
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

#define DATA_BLOCKS 65536
#define FILE_ENTRY_SIZE 288

#define SUPERBLOCK_START_BLOCK 0 //sizeof(Superblock) = 384 blocks or 196,608 byte
#define ROOT_START_BLOCK 384 //sizeof(RootDirectory) = 36 blocks or 18,432 byte
#define DATA_START_BLOCK 420

#define DMAP_SIZE 65536
#define FAT_SIZE 65536

// TODO: Add structures of your file system here
typedef struct {
	char filename[NAME_LENGTH];
	uint8_t uid;
	uint8_t gid;
	uint32_t sizeOfFile;
	mode_t mode;
	uint32_t ctime;
	uint32_t atime;
	uint32_t mtime;
	uint16_t firstBlock;
}FileEntry;

typedef struct {
	FileEntry fe;
	int blockRead;
	char data[BLOCK_SIZE];
}BlockCache;

#endif /* myfs_structs_h */
