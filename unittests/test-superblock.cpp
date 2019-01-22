#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Superblock.h"

TEST_CASE("Wird der naechste freie Block gefunden?", "[findFreeBlock]") 
{
	int ret;
	bool *DMap = (bool*) malloc(DMAP_SIZE);
	memset(DMap, false, DMAP_SIZE);
	uint16_t *FAT = (uint16_t*) malloc(FAT_SIZE);

	DMap[4] = true;
	Superblock *sb = new Superblock(DMap, FAT);
	ret = sb->findFreeBlock();
	REQUIRE(ret == 4);

	delete sb;

	DMap[4] = false; DMap[65535] = true;
	sb = new Superblock(DMap, FAT);
	ret = sb->findFreeBlock();
	REQUIRE(ret == 65535);
}

TEST_CASE("Wird der nachfolgende Block gefunden?", "[findNextBlock]")
{
	int ret;
	bool *DMap = (bool*) malloc(DMAP_SIZE);
	uint16_t *FAT = (uint16_t*) malloc(FAT_SIZE);
	memset(FAT, -1, FAT_SIZE);
	FAT[5] = 7;
	Superblock *sb = new Superblock(DMap, FAT);

	ret = sb->findNextBlock(5);
	REQUIRE(ret == 7);

	ret = sb->findNextBlock(7);
	REQUIRE(ret == 0);
}

TEST_CASE("Wird der nachfolgende Block richtig gesetzt?", "[setNextBlock]")
{
	int ret;
	bool *DMap = (bool*) malloc(DMAP_SIZE);
	uint16_t *FAT = (uint16_t*) malloc(FAT_SIZE);
	memset(FAT, -1, FAT_SIZE);
	Superblock *sb = new Superblock(DMap, FAT);

	sb->setNextBlock(5,8);
	REQUIRE(sb->findNextBlock(5) == 8);

	sb->setNextBlock(5, -1);
	REQUIRE(sb->findNextBlock(5) == 0);

	sb->setNextBlock(0, 3);
	REQUIRE(sb->findNextBlock(0) == 0);
}
