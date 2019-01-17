#include "Superblock.h"


Superblock::Superblock(bool *dmap, uint16_t *fat)
{
	memcpy(this->DMap, dmap, DMAP_SIZE);
	memcpy(this->FAT, fat, FAT_SIZE);
}

Superblock::Superblock()
{
	memset(this->DMap, 1, DMAP_SIZE * sizeof(bool));
	memset(this->FAT, 0, FAT_SIZE * sizeof(uint16_t));
}

Superblock::~Superblock()
{

}

/*
Finde den n?chsten freien Block in der DMap und gib die Adresse dessen zur?ck.
*/
uint16_t Superblock::findFreeBlock()
{
	uint8_t i = 1;
	while (DMap[i])
	{
		i++;
	}
	return i;
}

/*
Hat der Angegebene Block einen nachfolge Block? Falls ja
gib die Adresse zur?ck, falls nein gib 0 zur?ck.
*/
uint16_t Superblock::findNextBlock(uint16_t address)
{
	if (FAT[address]) return FAT[address];
	else return false;
}

/*
Setzt den n?chsten Block eines Blocks. Um den nachfolgenden
Block zu entfernen muss nextBlockAdress auf -1 gesetzt wird.
*/
void Superblock::setNextBlock(uint16_t blockAddress, uint16_t nextBlockAddress)
{
	if (blockAddress == 0) return;
	if (nextBlockAddress == -1) FAT[blockAddress] = 0;
	else FAT[blockAddress] = nextBlockAddress;
}

/*
Markiere Block als belegt (status = false) oder frei (status = true)
*/
void Superblock::markBlock(uint16_t address, bool status)
{
	DMap[address] = status;
}

/*
Serialisiere den gesamten Superblock (bestehend aus DMAP, FAT und ggf. weiteren Attributen)
sequentiell in den Buffer, welcher anschlie?end geschrieben werden kann.
*/
void Superblock::serialize(char* buffer)
{
	memcpy(buffer, this->DMap, DMAP_SIZE);
	memcpy(buffer + DMAP_SIZE, this->FAT, FAT_SIZE);
}

/*
Lese den Buffer ein und schreibe die einzelnen Elemente zur?ck in die Attribute des Superblocks.
*/
void Superblock::deserialize(char* buffer)
{
	memcpy(this->DMap, buffer, DMAP_SIZE);
	memcpy(this->FAT, buffer + DMAP_SIZE, FAT_SIZE);
}
