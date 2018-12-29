#include "RootDirectory.h"

RootDirectory::RootDirectory(uint16_t rootDirectoryStartBlock) {
	this.listDeserialzed = false;
	this.rootDirectoryStartBlock = rootDirectoryStartBlock;
	this.deserialize();
}

RootDirectory::~RootDirectory() {
	this->serialize();
}

int RootDirectory::addEntry(const char* path, mode_t mode) {
	FileEntry newEntry;
	//Remove Path information of filepath
	int i = 0;
	int lastIndex = 0;
	while (*path != '\0') {
		if (*path == '/' || *path == '\\' ){
			lastIndex = i;
		}
		i++;
		path++;
	}
	path -= lastIndex;
	
	//check if file already exists
	for (int i = 0; i < this->fileList.length; i++) {
		if (strcmp(this->fileList[i].filename, path)) {
			return -(EEXIST);
		}
	}

	//write entry
	newEntry.filename = path;
	newEntry.mode = mode;
	newEntry.atime = time();
	newEntry.ctime = time();
	newEntry.mtime = time();
	newEntry.gid;
	newEntry.uid;

	//save entry in list
	for (int i = 0; i < this->fileList.length; i++;) {
		if (this->fileList[i] == null) {
			this->fileList[i] = newEntry;
			i = this->fileList.length;
			return 1;
		}
	}

	return -(EIO);
	
}