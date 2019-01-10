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

int main(int argc, char *argv[]) {

    // TODO: Implement file system generation & copying of files here
    Superblock *sb = new Superblock;
    RootDirectory *rd = new RootDirectory;
    BlockDevice *bd = new BlockDevice;
    bd->open(argv[0]);

    rd->init();
    // TODO (optional): init Superblock with default values

    for(int i = 1; i < argc; i++){
    	uint16_t firstBlock = sb->findFreeBlock();
    	uint32_t sizeOfFile = 0;
    	sb->markBlock(firstBlock, true);
    	rd->addEntry(argv[i], firstBlock, sizeOfFile, 0444, getuid(), getgid());

    	// TODO: open real file and copy files into bd
    	//write file on blockdevice
    	for(int i = 0; i < ((sizeOfFile + BLOCK_SIZE) / BLOCK_SIZE); i++){
    		uint16_t nextBlock = sb->findFreeBlock();
    		if(i == 0){
    			// sb set next Block
    		}
    		else{
    			// sb set next Block
    		}
    	}
    }
    return 0;
}
