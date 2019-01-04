#include "../includes/RootDirectory.h"

RootDirectory::RootDirectory() {
	this->listDeserialized = false;
	this->deserialize();
}

RootDirectory::~RootDirectory() {
	this->serialize();
}

int RootDirectory::addEntry(const char* path, mode_t mode, uint8_t uid, uint8_t gid) {
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
	for (int i = 0; i < DATA_BLOCKS; i++) {
		if (strcmp(this->fileList[i].filename, path)) {
			return -(EEXIST);
		}
	}

	//write entry
	newEntry.filename = path;
	newEntry.mode = mode;

	struct timeval tv;
	gettimeofday(&tv,nullptr);
	newEntry.atime = (uint32_t) tv.tv_sec;
	newEntry.ctime = (uint32_t) tv.tv_sec;
	newEntry.mtime = (uint32_t) tv.tv_sec;
	newEntry.gid = gid;
	newEntry.uid = uid;

	//save entry in list
	for (int i = 0; i < DATA_BLOCKS; i++) {
		if (this->fileList[i] == 0) {
			this->fileList[i] = newEntry;
			i = DATA_BLOCKS;
			return 1;
		}
	}

	return -(EIO);
	
}
