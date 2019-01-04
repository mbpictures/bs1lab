#pragma once

#include "myfs-structs.h"
#include <string.h>
#include <errno.h>
#include <sys/time.h>



class RootDirectory {
	private:
		FileEntry fileList[65536];
		bool listDeserialized;
	public:
		RootDirectory();
		~RootDirectory();
		int addEntry(const char* path, mode_t mode, uint8_t uid, uint8_t gid);
		int removeEntry(const char* path);
		void serialize();
		void deserialize();
};
