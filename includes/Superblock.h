#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef Superblock_h
#define Superblock_h

#define OFFSET_ROOT 272
#define OFFSET_DATA 4880
#define OFFSET_ENDOFDATA 70416
#define DMAP_SIZE 65536
#define FAT_SIZE 65536

class Superblock {

public:
	uint32_t blockCount;
	bool DMap[DMAP_SIZE];
	uint16_t FAT[FAT_SIZE];
//public:

	Superblock();

	Superblock(bool dmap[], uint16_t fat[]);

	~Superblock();

	uint16_t findFreeBlock();

	uint16_t findNextBlock(uint16_t address);

	void setNextBlock(uint16_t blockAdress, uint16_t nextBlockAddress);

	void markBlock(uint16_t adress, bool status);

	void serialize(char* buffer);

	void deserialize(char* buffer);
};
/*
typedef struct
{
	bool DMap[DMAP_SIZE];
	uint16_t FAT[FAT_SIZE];
}SerializedSuperBlock;
*/

#endif // Superblock_h