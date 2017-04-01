#ifndef HAMMING_UTIL_HEADER
#define HAMMING_UTIL_HEADER

#define CODE_WORD_SIZE 63
#define CODE_DATA_SIZE 57
#define BITS_IN_BYTE 8

typedef struct file_decryption
{
	int received_counter;
	int wrote_counter;
	int corrected_counter;
} file_decryption;

void getDecryptedResult(char *output);

void removeCheckBits(char *buffer, char *output, int index);

#endif