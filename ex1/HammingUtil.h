#ifndef HAMMING_UTIL_HEADER
#define HAMMING_UTIL_HEADER

#define CODE_WORD_SIZE 63
#define CODE_DATA_SIZE 57
#define BITS_IN_BYTE 8

#define powerTwo(x) 1 << x

typedef struct file_decorder
{
	int received_counter;
	int wrote_counter;
	int corrected_counter;
} file_decorder;

void getDecodedResult(char *output);

void removeCheckBits(char *buffer, char *output, int index);

void hammingDecoder(int current_position, char *message);

#endif