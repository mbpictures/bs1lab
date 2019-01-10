#include "Superblock.h"


/*
Superblock()
{
	this->deserialize();
}

~Superblock()
{
	this->serialize();
}
*/

/*
Finde den nächsten freien Block in der DMap und gib die Adresse dessen zurück.
*/
uint16_t Superblock::findFreeBlock()
{
	uint8_t i = 1;
	while (DMap[i]) i++;
	return FAT[i];
}

/*
Hat der Angegebene Block einen nachfolge Block? Falls ja
gib die Adresse zurück, falls nein gib 0 zurück.
*/
uint16_t Superblock::findNextBlock(uint16_t address)
{
	if (FAT[address]) return FAT[address];
	else return false;
}

/*
Setzt den nächsten Block eines Blocks. Um den nachfolgenden
Block zu entfernen muss nextBlockAdress auf -1 gesetzt wird.
*/
void Superblock::setNextBlock(uint16_t blockAddress, uint16_t nextBlockAddress)
{
	if (blockAddress < 1) return;
	if (blockAddress == -1) FAT[blockAddress] = 0;
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
sequentiell in den Buffer, welcher anschließend geschrieben werden kann.
*/
void Superblock::serialize()
{
	SerializedSuperBlock s = { this->DMap, this->FAT };
	//printf("Serialize: %d", sizeof(s));
	char* buffer = new char[sizeof(s)];
	memcpy(&buffer, &s, sizeof(s));
}

/*
Lese den Buffer ein und schreibe die einzelnen Elemente zurück in die Attribute des Superblocks.
*/
void Superblock::deserialize()
{
	SerializedSuperBlock s;
	char* buffer = new char[sizeof(s)];
	memcpy(&s, &buffer, sizeof(s));
	//this.DMap = s.DMap;
}