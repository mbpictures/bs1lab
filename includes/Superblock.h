#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifndef Superblock_h
#define Superblock_h

#define OFFSET_ROOT 272
#define OFFSET_DATA 4880
#define OFFSET_ENDOFDATA 70416

class Superblock {

private:
	uint32_t blockCount;
	bool DMap[65536];
	uint16_t FAT[65536];
public:

	uint16_t findFreeBlock();

	uint16_t findNextBlock(uint16_t address);

	void setNextBlock(uint16_t blockAdress, uint16_t nextBlockAddress);

	void markBlock(uint16_t adress, bool status);

	void serialize();

	void deserialize();
};

typedef struct
{
	bool DMap[65536];
	uint16_t FAT[65536];
}SerializedSuperBlock;

#endif // Superblock_h