#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RootDirectory.h"

TEST_CASE("Initialisiert init fileList richtig?", "[init]")
{
	bool odd = false;
	RootDirectory *rd = new Rootdirectory();
	rd->init();

	for (int i = 0; i < NUM_DIR_ENTRIES; i++)
	{
		fileEntry fe = rd->fileList[i];
		if (!((fe.uid == 0) && (fe.gid == 0) && (fe.sizeOfFile == 0) &&(fe.mode == 0) &&(fe.ctime == 0) && (fe.atime == 0) && (fe.mtime == 0) && (fe.firstBlock == 0) && (fe.filename == '\0')))
		{
			odd = true;
		}
	}

	REQUIRE(odd == false);
}

TEST_CASE("Werden Entries richtig hinzugefügt?", "[addEntry]")
{
	int ret = false;
	bool odd = false;
	RootDirectory *rd = new Rootdirectory();
	rd->init();
	ret = rd->addEntry("/somefile.txt", 3, 12, 0777, 125, 254);
	fileEntry fe = rd->fileList[0];

		if (!((fe.uid == 125) && (fe.gid == 254) && (fe.sizeOfFile == 12) &&(fe.mode == 0777) && (fe.firstBlock == 3) && (fe.filename == '/somefile.txt')))
	{
		odd = true;
	}

	REQUIRE((ret == 0) && (odd == false));
}

TEST_CASE("Werden bestimmte Entries gefunden?", "[searchEntry]")
{
	int ret = false;
	RootDirectory *rd = new Rootdirectory();
	rd->init();
	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254); //4th Entry, so it should equal fileList[3]

	ret = rd->searchEntry("/somefile.txt", 0, 0);

	REQUIRE((ret == 3));
}

TEST_CASE("Wird die Dateigröße richtig gesetzt?", "[setSizeOfFile]")
{
	int ret = false;
	RootDirectory *rd = new Rootdirectory();
	rd->init();
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->setSizeOfFile(0, 32);

	if(fileList[0].sizeOfFile == 32)
	{
		true
	}

	REQUIRE((ret == true));
}

TEST_CASE("Werden alle vorhandene Entries gefunden?", "[getAllFiles]")
{
	int ret = false;
	RootDirectory *rd = new Rootdirectory();
	FileEntry *files;
	rd->init();
	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->getAllFiles(files);

	if ((files[0].filename != '\0') && (files[1].filename != '\0') && (files[2].filename != '\0') && (files[3].filename != '\0') && (files[4].filename == '\0'))
	{
		ret = true;
	}

	REQUIRE((ret == true));
}

TEST_CASE("Wir das RD richtig serialisiert und deserialisiert?", "[serialize/deserialize]")
{
	int ret = false;
	RootDirectory *rd = new Rootdirectory();
	char *buffer = new char[sizeof(FileEntry) * NUM_DIR_ENTRIES];
	rd->init();
	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->serialize(buffer);

	rd->addEntry("/soasdile.txt", 1, 1, 0777, 125, 254);
	rd->addEntry("/somaeere.txt", 2, 1, 0777, 125, 254);
	rd->addEntry("/somrere.txt", 3, 1, 0777, 125, 254);
	rd->addEntry("/somefile.txt", 4, 12, 0777, 125, 254);

	rd->deserialize(buffer);

	if((files[0].filename != '\0') && (files[1].filename != '\0') && (files[2].filename != '\0') && (files[3].filename != '\0') && (files[4].filename == '\0'))
	{
		ret = true;
	}

	REQUIRE((ret == true));
}
