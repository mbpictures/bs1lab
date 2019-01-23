#include "RootDirectory.h"
#include <iostream>

RootDirectory::RootDirectory() {
	this->listDeserialized = false;
}

RootDirectory::~RootDirectory() {
	//this->serialize();
}

void RootDirectory::init(){
	FileEntry fe;
	fe.filename[0] = '\0';
	fe.firstBlock = 0;
	fe.atime = 0;
	fe.ctime = 0;
	fe.gid = 0;
	fe.mode = 0;
	fe.mtime = 0;
	fe.sizeOfFile = 0;
	fe.uid = 0;
	for(int i = 0; i < NUM_DIR_ENTRIES; i++){
		this->fileList[i] = fe;
	}
}

int RootDirectory::addEntry(const char* path, uint16_t firstblock, uint32_t sizeOfFile, mode_t mode, uint8_t uid, uint8_t gid) {
	FileEntry *newEntry = new FileEntry;
	//Remove Path information of filepath
	char *escapedpath = strrchr(strdup(path), '/');
	if(escapedpath == 0){
		char *newEscaped = new char[strlen(path) + 2];
		strcpy(newEscaped, "/");
		strcat(newEscaped, path);
		escapedpath = newEscaped;
	}


	//check if file already exists
	for (int i = 0; i < NUM_DIR_ENTRIES; i++) {
		if (strcmp(this->fileList[i].filename, escapedpath) == 0) {
			return -(EEXIST);
		}
	}

	//write entry
	strcpy(newEntry->filename, escapedpath);
	newEntry->mode = mode;

	newEntry->filename[258] = '\0';
	struct timeval tv;
	gettimeofday(&tv,NULL);
	newEntry->atime = (uint32_t) tv.tv_sec;
	newEntry->ctime = (uint32_t) tv.tv_sec;
	newEntry->mtime = (uint32_t) tv.tv_sec;
	newEntry->gid = gid;
	newEntry->uid = uid;
	newEntry->firstBlock = firstblock;
	newEntry->sizeOfFile = sizeOfFile;

	//save entry in list
	for (int i = 0; i < NUM_DIR_ENTRIES; i++) {
		if (this->fileList[i].firstBlock == 0) {
			this->fileList[i] = *newEntry;
			i = NUM_DIR_ENTRIES +1;
			return 0;
		}
	}

	return -(EIO); //no free entry in fileList left!
	
}

int RootDirectory::searchEntry(const char* path, uint8_t uid, uint8_t gid){
	for(int i = 0; i < NUM_DIR_ENTRIES; i++) {
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
	for(int i = 0; i < NUM_DIR_ENTRIES; i++) {
		if(strcmp(this->fileList[i].filename, path) == 0){
			this->fileList[i] = *empty;
			return 0;
		}
	}
	return -(ENOENT);
}

void RootDirectory::setSizeOfFile(int index, uint32_t size){
	this->fileList[index].sizeOfFile = size;
}

void RootDirectory::getAllFiles(FileEntry fes[]){
	fes = this->fileList;
}

void RootDirectory::serialize(char* buffer){
	//std::cout << "Buffer size: " << sizeof(*buffer) << " MEmcpy: " << sizeof(FileEntry) * NUM_DIR_ENTRIES;


	memcpy(buffer, this->fileList, sizeof(FileEntry) * NUM_DIR_ENTRIES);
	return;
}

void RootDirectory::deserialize(char* buffer){
	memcpy(this->fileList, buffer, FILE_ENTRY_SIZE * NUM_DIR_ENTRIES);
	return;
}
