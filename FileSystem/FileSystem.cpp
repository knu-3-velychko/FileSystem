#include "FileSystem.h"

const int BLOCKS = 64;
const int BLOCK_SIZE = 64;      // 64 bytes = 16 integers
const int BITMAP_SIZE = 64;     // 64 bits = 2 integers
const int DESCRIPTOR_SIZE = 4;  // 4 integers
const int FILE_LENGTH = 1;      // 1 int

const int NUMBER_OF_DESCRIPTORS = 24;   // 6 blocks
const int MAX_FILE_NAME_LENGTH = 4; // 4 chars
const int DIRECTORY_ENTRY = 2;  // 2 integers
const int DESCRIPTOR_INDEX = 1; // 1 integer

FileSystem::FileSystem() {

}
