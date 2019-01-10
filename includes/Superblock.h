#include <stdint.h>
#ifndef Superblock_h
#define Superblock_h

#define OFFSET_ROOT 272
#define OFFSET_DATA 4880
#define OFFSET_ENDOFDATA 70416

class Superblock {

	 private:
		uint32_t blockCount;
		bool DMap[2E16];
		uint16_t FAT[2E16];
	public:

		 uint16_t findFreeBlock();

		 uint16_t findNextBlock(uint16_t address);

		 void setNextBlock(uint16_t blockAdress, uint16_t nextBlockAddress);

		 void markBlock(uint16_t adress, Boolean status);

		 void serialize(const char* buffer);

		 void deserialize(const char* buffer);
};

#endif // Superblock_h