#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Superblock.h"

TEST_CASE("Wird der naechste freie Block gefunden?", "[findFreeBlock]") 
{
	int ret;
	bool DMap[DMAP_SIZE];
	uint16_t FAT[FAT_SIZE];

	DMap[4] = true;
	Superblock sp = new Superblock(DMap, FAT);
	ret = sp.findFreeBlock();
	REQUIRE(ret == 4);

	delete sp;

	DMap[4] = false; DMap[65535] = true;
	Superblock sp = new Superblock(DMap, FAT);
	ret = sp.findFreeBlock();
	REQUIRE(ret == 65535);
}

TEST_CASE("Wird der nachfolgende Block gefunden?", "[findNextBlock]")
{
	int ret;
	bool DMap[DMAP_SIZE];
	uint16_t FAT[FAT_SIZE];
	FAT[5] = 7;
	Superblock sp = new Superblock(DMap, FAT);

	ret = sp.findNextBlock(5);
	REQUIRE(ret == 7);

	ret = sp.findNextBlock(7);
	REQUIRE(ret == 0);
}

TEST_CASE("Wird der nachfolgende Block richtig gesetzt?", "[setNextBlock]")
{
	int ret;
	bool DMap[DMAP_SIZE];
	uint16_t FAT[FAT_SIZE];
	Superblock sp = new Superblock(DMap, FAT);

	sp.setNextBlock(5,8);
	REQUIRE(sp.findNextBlock(5) == 8);

	sp.setNextBlock(5, -1);
	REQUIRE(sp.findNextBlock(5) == 0);

	sp.setNextBlock(0, 3);
	REQUIRE(sp.findNextBlock(0) == 0);
}
