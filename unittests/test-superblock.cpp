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

TEST_CASE("Wird ein bestimmter Block in der DMap richtig gesetzt?", "[markBlock]")
{
	int ret;
	bool *DMap = (bool*) malloc(DMAP_SIZE);
	uint16_t *FAT = (uint16_t*) malloc(FAT_SIZE);
	memset(DMap, false, DMAP_SIZE);
	Superblock *sb = new Superblock(DMap, FAT);

	sb->markBlock(9, true);
	REQUIRE(sb->findFreeBlock() == 9);

	sb->markBlock(5, true);
	REQUIRE(sb->findNextBlock() == 5);

	sb->markBlock(5, false);
	REQUIRE(sb->findNextBlock() == 9);
}

TEST_CASE("Wir das SB richtig serialisiert und deserialisiert?", "[serialize/deserialize]")
{
	bool *dmap = (bool*) malloc(DMAP_SIZE);
	memset(dmap, 1, DMAP_SIZE);
	uint16_t *fat = (uint16_t*) malloc(FAT_SIZE);
	memset(fat, 0, FAT_SIZE);

	dmap[0] = 0;
	dmap[1] = 0;
	dmap[2] = 0;
	dmap[3] = 0;
	dmap[4] = 0;

	fat[0] = 127;
	fat[1] = 3;
	fat[2] = 222;
	fat[3] = 2;

	Superblock *sp = new Superblock(dmap, fat);
	char* buffer = (char*)malloc(DMAP_SIZE + FAT_SIZE);
	sp->serialize(buffer);

	sp->DMap[0] = 0;
	sp->DMap[1] = 0;
	sp->DMap[2] = 0;
	sp->DMap[3] = 1;
	sp->DMap[4] = 0;

	sp->FAT[0] = 1;
	sp->FAT[1] = 1;
	sp->FAT[3] = 1;
	sp->FAT[2] = 1;

	sp->deserialize(buffer);

	REQUIRE(sb->findFreeBlock() == 5);
	REQUIRE(sb->findNextBlock(2) == 222);
}
