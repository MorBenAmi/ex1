#ifndef HAMMING_UTIL_HEADER
#define HAMMING_UTIL_HEADER

#define CODE_WORD_SIZE 63
#define CODE_DATA_SIZE 57
#define BITS_IN_BYTE 8

#define powerTwo(x) 1 << x

void removeCheckBits(char *message, char *output, int index);

int hammingDecoder(int current_position, char *message);

#endif