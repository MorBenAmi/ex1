#include "HammingUtil.h"
#include <stdio.h>

#define PARITY_SIZE 6

#define get_bit(buffer, position) (buffer[position / BITS_IN_BYTE] >> (position % BITS_IN_BYTE)) & 1

int getErrorPosition(int *check_bits);

void hammingDecoder(int current_position, char *buffer);

//Local variable
file_decryption decrypted_file;

int getErrorPosition(int *check_bits)
{
	int i;
	int result = 0;

	for (i = 0; i < PARITY_SIZE; i++)
		result += (check_bits[i] * pow(2, i));

	return result;
}

void hammingDecoder(int current_position, char *buffer)
{
	int check_bits[PARITY_SIZE] = { 0 };
	int i;
	int error_position;

	for (i = 0; i < PARITY_SIZE; i++)
	{
		int parity_index = pow(2, i);
		int j;
		for (j = parity_index; j < CODE_WORD_SIZE; j += parity_index)
		{
			int k;
			for (k = 0; k < parity_index; k++, j++)
			{
				check_bits[i] ^= get_bit(buffer, current_position + (j - 1));
			}
		}
	}
	error_position = getErrorPosition(check_bits);

	if (error_position > 0)
	{
		//Error occured at error_position
		buffer[current_position + (error_position - 1)] = ~buffer[current_position + (error_position - 1)];
		decrypted_file.corrected_counter++;
	}

	decrypted_file.wrote_counter += CODE_DATA_SIZE;
	decrypted_file.received_counter += CODE_WORD_SIZE;
}

void getDecryptedResult(char *output)
{
	sprintf_s(output, "received: %d bytes\wrote: %d bytes\corrected: %d errors", decrypted_file.corrected_counter);
}

void removeCheckBits(char *buffer, char *output, int index)
{
	int i;
	for (i = 0; i < CODE_WORD_SIZE; i++)
	{

	}
}

