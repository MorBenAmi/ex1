#ifndef INPUT_HANDLER_HEADER
#define INPUT_HANDLER_HEADER

typedef struct file_decorder
{
	int received_counter;
	int wrote_counter;
	int corrected_counter;
} file_decorder;

file_decorder getDecodedFile();

void decodeMessage(char *message);

#endif