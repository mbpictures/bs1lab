#pragma once

#include "myfs-structs.h"
#include <string.h>
#include <errno.h>
#include <sys/time.h>



class RootDirectory {
	public:
		FileEntry fileList[NUM_DIR_ENTRIES];
		bool listDeserialized;
	public:
		RootDirectory();
		~RootDirectory();
		void init();
		int addEntry(const char* path, uint16_t firstblock, uint32_t sizeOfFile, mode_t mode, uint8_t uid, uint8_t gid);
		/**
		 * return index of fileentry matching path, negative int when error occured
		 */
		int searchEntry(const char* path, uint8_t uid, uint8_t gid);
		FileEntry getEntry(int index);
		int removeEntry(const char* path);
		void getAllFiles(FileEntry *fes);
		void serialize(char *buffer);
		void deserialize(char buffer[]);
};
