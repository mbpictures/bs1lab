#pragma once
#include <stdint.h>
#include <ctime>

typedef struct {
	char filename[8];
	uint8_t uid;
	uint8_t gid;
	uint16_t mode;
	uint32_t ctime;
	uint32_t atime;
	uint32_t mtime;
	uint16_t firstBlock;
}FileEntry;

class RootDirectory {
	private:
		FileEntry fileList[65536];
		boolean listDeserialized;
		uint16_t rootDirectoryStartBlock;
	public:
		public RootDirectory(uint_16_t rootDirectoryStartBlock);
		public ~RootDirectory();
		public int addEntry(const char* path, mode_t mode);
		public int removeEntry(const char* path);
		public void serialize(uint_16_t rootDirectoryStartBlock);
		public void deserialize(uint_16_t rootDirectoryStartBlock);
};