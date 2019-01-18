//
//  myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment this to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES

#include "macros.h"

#include "myfs.h"
#include "myfs-info.h"

MyFS* MyFS::_instance = NULL;

MyFS* MyFS::Instance() {
    if(_instance == NULL) {
        _instance = new MyFS();
    }
    return _instance;
}

MyFS::MyFS() {
	this->rd = new RootDirectory;
    this->logFile= stderr;
    this->openedFiles = 0;
    this->bd = new BlockDevice();
    this->sb = new Superblock();
}

MyFS::~MyFS() {
	this->serializeControlStructures();
	delete rd;
	delete sb;
	delete bd;
}

int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();
    LOGF("Getattr of Path: %s", path);
    if(strcmp(path, "/") == 0){ //getAttr of Root path
    	LOG("Return getAttr of root");
    	struct timeval tv;
    	gettimeofday(&tv,nullptr);
    	statbuf->st_atim.tv_sec = tv.tv_sec;
    	statbuf->st_ctim.tv_sec = tv.tv_sec;
    	statbuf->st_mtim.tv_sec = tv.tv_sec;
    	statbuf->st_mode = S_IFDIR | 0755;
    	statbuf->st_nlink = 1;
    	statbuf->st_size = (size_t) DATA_BLOCKS * BLOCK_SIZE;
    	statbuf->st_blocks = DATA_BLOCKS; //round up
    	statbuf->st_gid = getgid();
    	statbuf->st_uid = getuid();
    	RETURN(0);
    }
    int index = this->rd->searchEntry(path, getuid(), getgid());
    if(index >= 0){
    	FileEntry fe = this->rd->getEntry(index);
    	statbuf->st_atim.tv_sec = fe.atime;
    	statbuf->st_ctim.tv_sec = fe.ctime;
    	statbuf->st_mtim.tv_sec = fe.mtime;
    	statbuf->st_mode = fe.mode;
    	statbuf->st_size = (size_t) fe.sizeOfFile;
    	statbuf->st_blocks = (fe.sizeOfFile + BLOCK_SIZE - 1) / BLOCK_SIZE; //round up
    	statbuf->st_gid = fe.gid;
    	statbuf->st_uid = fe.uid;
    	RETURN(0);
    }

    RETURN(index);
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();
    int firstBlock = 0; // TODO: find free block in dmap
    int sizeOfFile = 0;
    int status = this->rd->addEntry(path, firstBlock, sizeOfFile, mode, getuid(), getgid());
    
    this->serializeControlStructures();

    RETURN(status);
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseUnlink(const char *path) {
    LOGM();
    int index = this->rd->searchEntry(path, getuid(), getgid());
    if(index < 0){ //file doesn't exist!
    	RETURN(index);
    }
    FileEntry fe = this->rd->getEntry(index);
    uint16_t firstBlock = fe.firstBlock;
    this->sb->markBlock(firstBlock, 0);
    // TODO: mark all blocks of file as FREE & clean fat for this file
    int status = this->rd->removeEntry(path);
    
    this->serializeControlStructures();

    RETURN(status);
}

int MyFS::fuseRmdir(const char *path) {
    LOGM();
    return 0;
}

int MyFS::fuseSymlink(const char *path, const char *link) {
    LOGM();
    return 0;
}

int MyFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseLink(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    return 0;
}

int MyFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    return 0;
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return 0;
}

int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    if(this->openedFiles == NUM_OPEN_FILES){
    	LOG("to many files opened");
    	RETURN(-(ENFILE));
    }

    int index = this->rd->searchEntry(path, getuid(), getgid());
    if(index >= 0){
    	FileEntry fe = this->rd->getEntry(index);
    	BlockCache *cache = new BlockCache();
    	cache->blockRead = -1; //no block read yet
    	cache->fe = fe;
    	//cache->data;
    	fileInfo->fh = (uint64_t) cache;
    	this->openedFiles++;
    	LOGF("File opened: %s", path);
    	return 0;
    }
    
    RETURN(index);
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    LOGF("BlockCache: %d", (int) fileInfo->fh);
    if(fileInfo->fh != 0){
    	BlockCache *cache = (BlockCache*) fileInfo->fh;
    	if(strcmp(cache->fe.filename, path) == 0){
    		u_int32_t blockNo = 0;
    		if(cache->blockRead == -1){
    			blockNo = (u_int32_t) cache->fe.firstBlock;
    		}
    		else{
    			// TODO: iterate through fat until offset is reached to open read next block
    			blockNo = 0;
    		}
    		bd->read(blockNo, cache->data);

    		int j = 0;
    		for(int i = ((int) offset / BLOCK_SIZE); i <= (((int) offset / BLOCK_SIZE) + (int) size); i++){
    			buf[j] = cache->data[i];
    			j++;
    		}
    		RETURN(j); //return the amount of read bytes
    	}
    }

    RETURN(-1);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    
    uint16_t blockNo = (offset + BLOCK_SIZE - 1) / BLOCK_SIZE; //BlockNo to write. 1 = write first block of device, or write nth block of file
    if(blockNo); //remove
    int index = this->rd->searchEntry(path, getuid(), getgid());
    FileEntry fe = this->rd->getEntry(index);
    uint16_t firstBlock = fe.firstBlock;
    if(firstBlock); //remove

    //TODO: iterate through fat until blockNo is reached, if file doesn't have blockNo blocks,
    // 		allocate more blocks using fat and dmap
    // 		when physical block is found, write buf to BlockDevice using offset % BLOCK_SIZE and size


    this->serializeControlStructures();

    RETURN(0);
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    if(fileInfo->fh != 0){
    	BlockCache *cache = (BlockCache*) fileInfo->fh;
    	if(strcmp(cache->fe.filename, path) == 0){
    		delete cache;
    		this->openedFiles--;
    		RETURN(0);
    	}
    	else{
    		RETURN(-ENOENT);
    	}
    }
    RETURN(-20);
}

int MyFS::fuseFsync(const char *path, int datasync, struct fuse_file_info *fi) {
    LOGM();
    return 0;
}

int MyFS::fuseListxattr(const char *path, char *list, size_t size) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseRemovexattr(const char *path, const char *name) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseOpendir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    //check rights to access directory
    fileInfo->fh = 1;
    
    RETURN(0);
}

int MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    
    filler(buf, ".", NULL, 0); //add self directory
    filler(buf, "..", NULL, 0); //add top directory

    if(strcmp(path, "/") == 0){
    	FileEntry fes[NUM_DIR_ENTRIES];
    	rd->getAllFiles(fes);
    	for(int i = 0; i < NUM_DIR_ENTRIES; i++){
    		if(strcmp(fes[i].filename, "\0") != 0 && fes[i].firstBlock != 0){
    			struct stat s;
    			s.st_atim.tv_sec = fes[i].atime;
    			s.st_ctim.tv_sec = fes[i].ctime;
    			s.st_mtim.tv_sec = fes[i].mtime;
    			s.st_mode = fes[i].mode;
    			s.st_size = (size_t) fes[i].sizeOfFile;
    			s.st_blocks = (fes[i].sizeOfFile + BLOCK_SIZE - 1) / BLOCK_SIZE; //round up
    			s.st_gid = fes[i].gid;
    			s.st_uid = fes[i].uid;

    			const struct stat *x = &s;
    			filler(buf, fes[i].filename, x, 0);
    		}
    	}
    	RETURN(0);
    }
    
    RETURN(-ENOENT);

    

    // <<< My new code
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    
    // TODO: Implement this!
    
    RETURN(0);
}

int MyFS::fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();
    
    int r = this->fuseMknod(path, mode, 0);
    if(r >= 0){
    	int r2 = this->fuseOpen(path, fileInfo);
    	if(r2 >= 0){
    		RETURN(0);
    	}
    	else{
    		RETURN(r2)
    	}
    }
    RETURN(r);
}

void MyFS::fuseDestroy() {
    LOGM();
}

void* MyFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
    if(this->logFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
    } else {
        //    this->logFile= ((MyFsInfo *) fuse_get_context()->private_data)->logFile;
        
        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);
        
        LOG("Starting logging...\n");
        LOGM();
        
        // you can get the containfer file name here:
        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);
        
        //open BlockDevice
        bd->open(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        LOG("Read Root Directory");
        //deserialize RootDirectory
        char *bufferRD = new char[FILE_ENTRY_SIZE * NUM_DIR_ENTRIES];
        for(int i = ROOT_START_BLOCK; i < DATA_START_BLOCK; i++){
        	bd->read(i, bufferRD);
           	bufferRD += BLOCK_SIZE;
        }
        bufferRD -= FILE_ENTRY_SIZE * NUM_DIR_ENTRIES;

        LOG("Read Superblock");
        int size = (sizeof(uint16_t) * DATA_BLOCKS) + (sizeof(bool) * DATA_BLOCKS);

        char *bufferSB = new char[size];
        for(int i = SUPERBLOCK_START_BLOCK; i < ROOT_START_BLOCK; i++){
        	bd->read(i, bufferSB);
           	bufferSB += BLOCK_SIZE;
        }
        bufferSB -= size;
        this->sb->deserialize(bufferSB);

        //just read first entry for test. SPOILER: doesn't work
        int index = this->rd->searchEntry("/README.md", getuid(), getgid());
        LOGF("index: %d", index);
   }
    
    RETURN(0);
}

#ifdef __APPLE__
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x) {
#else
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
#endif
    LOGM();
    RETURN(0);
}
    
#ifdef __APPLE__
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size, uint x) {
#else
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size) {
#endif
    LOGM();
    RETURN(0);
}
        
// TODO: Add your own additional methods here!
void MyFS::serializeControlStructures(){
	//write RootDirectory to Blockdevice
	char *bufferRD = new char[sizeof(FileEntry) * NUM_DIR_ENTRIES];
	this->rd->serialize(bufferRD);
	for(int i = ROOT_START_BLOCK; i < DATA_START_BLOCK; i++){
	   	char writeBuf[BLOCK_SIZE];
	   	for(int i = 0; i < BLOCK_SIZE; i++){
	   		writeBuf[i] = *bufferRD;
	   		bufferRD++;
	   	}
	  	this->bd->write(i, writeBuf);
	}

	//write Superblock to Blockdevice
	char *bufferSB = new char[(ROOT_START_BLOCK -1) * BLOCK_SIZE];
	this->sb->serialize(bufferSB);
	for(int i = SUPERBLOCK_START_BLOCK; i < ROOT_START_BLOCK; i++){
	   	char writeBuf[BLOCK_SIZE];
	   	for(int i = 0; i < BLOCK_SIZE; i++){
	   		writeBuf[i] = *bufferSB;
	   		bufferSB++;
	   	}
	   	this->bd->write(i, writeBuf);
	}
}

