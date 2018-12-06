//
//  test-blockdevice.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <stdio.h>
#include <string.h>

#include "helper.hpp"

#include "blockdevice.h"

#define BD_PATH "/tmp/bd.bin"
#define NUM_TESTBLOCKS 1024

TEST_CASE( "Write/Read blocks", "[blockdevice]" ) {
    
    remove(BD_PATH);
    
    BlockDevice bd;
    bd.create(BD_PATH);
    
    SECTION("writing single block") {
        char* r= new char[BD_BLOCK_SIZE];
        memset(r, 0, BD_BLOCK_SIZE);
        
        char* w= new char[BD_BLOCK_SIZE];
        gen_random(w, BD_BLOCK_SIZE);
        
        bd.write(0, w);
        bd.read(0, r);
        
        REQUIRE(memcmp(w, r, BD_BLOCK_SIZE) == 0);

        delete [] r;
        delete [] w;
    }
    
    SECTION("writing multiple blocks") {
        char* r= new char[BD_BLOCK_SIZE * NUM_TESTBLOCKS];
        memset(r, 0, BD_BLOCK_SIZE * NUM_TESTBLOCKS);
        
        char* w= new char[BD_BLOCK_SIZE * NUM_TESTBLOCKS];
        gen_random(w, BD_BLOCK_SIZE * NUM_TESTBLOCKS);
        
        // write all blocks
        for(int b= 0; b < NUM_TESTBLOCKS; b++) {
            bd.write(b, w + b*BD_BLOCK_SIZE);
        }
        
        // read all blocks
        for(int b= 0; b < NUM_TESTBLOCKS; b++) {
            bd.read(b, r + b*BD_BLOCK_SIZE);
        }

        REQUIRE(memcmp(w, r, BD_BLOCK_SIZE * NUM_TESTBLOCKS) == 0);
    }
    
    bd.close();
    remove(BD_PATH);
    
}
