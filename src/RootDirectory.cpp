#include "RootDirectory.h"
#include <iostream>

RootDirectory::RootDirectory() {
	this->listDeserialized = false;
	this->deserialize();
}

RootDirectory::~RootDirectory() {
	this->serialize();
}

int RootDirectory::addEntry(const char* path, uint16_t firstblock, uint32_t sizeOfFile, mode_t mode, uint8_t uid, uint8_t gid) {
	FileEntry *newEntry = new FileEntry;
	//Remove Path information of filepath
	char *escapedpath = strrchr(strdup(path), '/');
	
	//check if file already exists
	for (int i = 0; i < DATA_BLOCKS; i++) {
		if (strcmp(this->fileList[i].filename, escapedpath) == 0) {
			return -(EEXIST);
		}
	}

	//write entry
	strcpy(newEntry->filename, escapedpath);
	newEntry->mode = mode;

	struct timeval tv;
	gettimeofday(&tv,nullptr);
	newEntry->atime = (uint32_t) tv.tv_sec;
	newEntry->ctime = (uint32_t) tv.tv_sec;
	newEntry->mtime = (uint32_t) tv.tv_sec;
	newEntry->gid = gid;
	newEntry->uid = uid;
	newEntry->firstBlock = firstblock;
	newEntry->sizeOfFile = sizeOfFile;

	//save entry in list
	for (int i = 0; i < DATA_BLOCKS; i++) {
		if (this->fileList[i].firstBlock == 0) {
			this->fileList[i] = *newEntry;
			i = DATA_BLOCKS;
			return 1;
		}
	}

	return -(EIO);
	
}

int RootDirectory::searchEntry(const char* path, uint8_t uid, uint8_t gid){
	for(int i = 0; i < DATA_BLOCKS; i++) {
		if(strcmp(this->fileList[i].filename, path) == 0){
			return i;
		}
	}
	return -(ENOENT);
}

FileEntry RootDirectory::getEntry(int index){
	if(index >= 0){
		return this->fileList[index];
	}
	else{
		FileEntry *fe = new FileEntry;
		return *fe;
	}
}

int RootDirectory::removeEntry(const char* path){
	FileEntry *empty = new FileEntry;
	empty->firstBlock = 0;
	empty->filename[0] = '\0';
	for(int i = 0; i < DATA_BLOCKS; i++) {
		if(strcmp(this->fileList[i].filename, path) == 0){
			this->fileList[i] = *empty;
			return 1;
		}
	}
	return -(ENOENT);
}

void RootDirectory::serialize(){
	return;
}

void RootDirectory::deserialize(){
	return;
}
