#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "myfs-structs.h"

#ifndef Superblock_h
#define Superblock_h

class Superblock {

public:
	uint32_t blockCount;
	bool DMap[DMAP_SIZE];
	uint16_t FAT[FAT_SIZE];
//public:

	Superblock();

	Superblock(bool *dmap, uint16_t *fat);

	~Superblock();

	uint16_t findFreeBlock();

	uint16_t findNextBlock(uint16_t address);

	void setNextBlock(uint16_t blockAdress, uint16_t nextBlockAddress);

	void markBlock(uint16_t adress, bool status);

	void serialize(char* buffer);

	void deserialize(char* buffer);
};

#endif // Superblock_h
