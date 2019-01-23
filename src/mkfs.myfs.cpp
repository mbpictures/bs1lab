//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include <iostream>
#include <stdio.h>

int main(int argc, char *argv[]) {

	std::cout << "Program start: " << argc << std::endl;
    // TODO: Implement file system generation & copying of files here
    Superblock *sb = new Superblock;
    std::cout << "Superblock created\n";
    RootDirectory *rd = new RootDirectory;
    std::cout << "RootDirectory created\n";
    BlockDevice *bd = new BlockDevice(BLOCK_SIZE);
    std::cout << "BlockDevice created\n";
    std::cout << "Write container: " << argv[1] << std::endl;

    bd->create(argv[1]);


    rd->init();

    //add defaults to root directory
    /*
    rd->addEntry("/", 0, 0, 0444, getuid(), getgid());
    rd->addEntry("/.Trash", 0, 0, 0444, getuid(), getgid());
    rd->addEntry("/.Trash-1000", 0, 0, 0444, getuid(), getgid());
	*/

    uint16_t nextBlock = 1;
    for(int i = 2; i < argc; i++){

    	int fd = open(argv[i], O_RDONLY); //open file for Read-Only
    	//get file of size
    	struct stat stat_buf;
    	int sc = fstat(fd, &stat_buf);
    	uint32_t sizeOfFile = sc == 0 ? stat_buf.st_size : 0;

    	sb->markBlock(nextBlock, 0);

    	//get Filename
    	size_t filenameLength = strlen(argv[i]);
    	char *filename = (char*) malloc(filenameLength + 1);
    	strcpy(filename, argv[i]);
    	filename[filenameLength] = '\0';

    	const char *filenameConst = filename;

    	std::cout << "Writing File: " << filename << " Size: " << sizeOfFile << std::endl;

    	rd->addEntry(filenameConst, nextBlock, sizeOfFile, 0444, getuid(), getgid());

    	char *buf = new char[sizeOfFile];
    	read(fd, buf, sizeOfFile);
    	//write file on blockdevice
    	for(unsigned int i = 0; i < ((sizeOfFile + BLOCK_SIZE) / BLOCK_SIZE); i++){
    		uint16_t nextBlockNew = nextBlock + 1;
    		sb->setNextBlock(nextBlock, nextBlockNew);
    		sb->markBlock(nextBlockNew, 0);

    		char writeBuf[BLOCK_SIZE];
    		for(int i = 0; i < BLOCK_SIZE; i++){
    			writeBuf[i] = *buf;
    			buf++;
    		}

    		bd->write(nextBlock + DATA_START_BLOCK -1, writeBuf);
    		nextBlock = nextBlockNew;
    	}
    }
    //last block
    char data[BLOCK_SIZE];
    bd->write(DATA_START_BLOCK+DATA_BLOCKS, data);

    //write RootDirectory to Blockdevice
    char *buffer = new char[sizeof(FileEntry) * NUM_DIR_ENTRIES];
    rd->serialize(buffer);

    for(int i = ROOT_START_BLOCK; i < DATA_START_BLOCK; i++){
    	char writeBuf[BLOCK_SIZE];
    	for(int i = 0; i < BLOCK_SIZE; i++){
    		writeBuf[i] = *buffer;
    		buffer++;
    	}
    	bd->write(i, writeBuf);
    }

    //write Superblock to Blockdevice
    char *bufferSB = new char[(ROOT_START_BLOCK -1) * BLOCK_SIZE];
    sb->serialize(bufferSB);
    for(int i = SUPERBLOCK_START_BLOCK; i < ROOT_START_BLOCK; i++){
      	char writeBuf[BLOCK_SIZE];
       	for(int i = 0; i < BLOCK_SIZE; i++){
       		writeBuf[i] = *bufferSB;
       		bufferSB++;
       	}
       	bd->write(i, writeBuf);
    }

    return 0;
}
