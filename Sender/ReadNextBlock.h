#ifndef READ_NEXT_BLOCK_HEADER
#define READ_NEXT_BLOCK_HEADER

#include <windows.h>

BOOL ReadNextBlock(FILE *file, unsigned char block[8]);

#endif