#include "catch.hpp"
#include "RootDirectory.h"
#include "helper.hpp"
#include <string.h>

TEST_CASE("Initialisiert init fileList richtig?", "[init]")
{
	bool odd;
	RootDirectory *rd = new RootDirectory();
	rd->init();

	for (int i = 0; i < NUM_DIR_ENTRIES; i++)
	{
		FileEntry fe = rd->fileList[i];
		odd = false;
		if ((fe.uid == 0) && (fe.gid == 0) && (fe.sizeOfFile == 0) &&(fe.mode == 0) &&(fe.ctime == 0) && (fe.atime == 0) && (fe.mtime == 0) && (fe.firstBlock == 0) && (fe.filename == '\0'))
		{
			odd = true;
		}
		REQUIRE(odd == false);
	}
}

TEST_CASE("Werden Entries richtig hinzugefügt?", "[addEntry]")
{
	int ret = false;
	bool odd = false;
	RootDirectory *rd = new RootDirectory();
	rd->init();
	ret = rd->addEntry("/somefile.txt", 3, 12, 0777, 125, 254);
	FileEntry fe = rd->fileList[0];

		if (!((fe.uid == 125) && (fe.gid == 254) && (fe.sizeOfFile == 12) &&(fe.mode == 0777) && (fe.firstBlock == 3) && (strcmp(fe.filename, "/somefile.txt") == 0)))
	{
		odd = true;
	}

	REQUIRE(ret == 0);
	REQUIRE(odd == false);
}

TEST_CASE("Werden bestimmte Entries gefunden?", "[searchEntry]")
{
	int ret = false;
	RootDirectory *rd = new RootDirectory();
	rd->init();
	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254); //4th Entry, so it should equal fileList[3]

	ret = rd->searchEntry("/somefile.txt", 0, 0);

	REQUIRE(ret == 3);
}

TEST_CASE("Wird die Dateigröße richtig gesetzt?", "[setSizeOfFile]")
{
	int ret = false;
	RootDirectory *rd = new RootDirectory();
	rd->init();
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->setSizeOfFile(0, 32);

	if(rd->fileList[0].sizeOfFile == 32)
	{
		ret = true;
	}

	REQUIRE(ret == true);
}

TEST_CASE("Werden alle vorhandene Entries gefunden?", "[getAllFiles]")
{
	int ret = false;
	RootDirectory *rd = new RootDirectory();
	FileEntry *files = (FileEntry*) malloc(sizeof(FileEntry) * NUM_DIR_ENTRIES);
	rd->init();
	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->getAllFiles(files);

	for(int i = 0; i < 4; i++)
	{
		ret = false;
		if ((files[i].filename != '\0'))
		{
			ret = true;
		}
		REQUIRE(ret == true);
	}
}

TEST_CASE("Wir das RD richtig serialisiert und deserialisiert?", "[serialize/deserialize]")
{
	int ret = false;
	RootDirectory *rd = new RootDirectory();
	char *buffer = new char[sizeof(FileEntry) * NUM_DIR_ENTRIES];
	rd->init();
	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->serialize(buffer);

	rd->addEntry("/soasssadile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somrrgaeere.txt", 2, 1, 0777, 125, 254);

	rd->deserialize(buffer);

	ret = rd->searchEntry("/soasdile.txt", 0, 0);
	REQUIRE(ret == 0);

	ret = rd->searchEntry("/somaeere.txt", 0, 0);
	REQUIRE(ret == 1);

	ret = rd->searchEntry("/somrere.txt", 0, 0);
	REQUIRE(ret == 2);

	ret = rd->searchEntry("/somefile.txt", 0, 0);
	REQUIRE(ret == 3);

	ret = rd->searchEntry("/soasssadile.txt", 0, 0);
	REQUIRE(ret == -(ENOENT));

	ret = rd->searchEntry("/somrrgaeere.txt", 0, 0);
	REQUIRE(ret == -(ENOENT));
}
