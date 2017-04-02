#define _CRT_SECURE_NO_WARNINGS
#include "HammingUtil.h"
#include <stdio.h>

#define PARITY_SIZE 6

#define isPowerOfTwo(x) (((x) != 0) && !((x) & ((x) - 1)))

void setBit(char *buffer, int position, int bit);

int getErrorPosition(int *check_bits);

void hammingDecoder(int current_position, char *buffer);

int getBit(char *buffer, int position);

//Local variable
file_decorder Decoded_file;

int getErrorPosition(int *check_bits)
{
	int i;
	int result = 0;

	for (i = 0; i < PARITY_SIZE; i++)
		result += (check_bits[i] * powerTwo(i));

	return result;
}

void hammingDecoder(int current_position, char *buffer)
{
	int check_bits[PARITY_SIZE] = { 0 };
	int i;
	int error_position;

	for (i = 0; i < PARITY_SIZE; i++)
	{
		int parity_index = powerTwo(i);
		int j;
		for (j = parity_index; j <= CODE_WORD_SIZE; j += parity_index)
		{
			int k;
			for (k = 0; k < parity_index; k++, j++)
				check_bits[i] ^= getBit(buffer, current_position + (j - 1));
		}
	}
	error_position = getErrorPosition(check_bits);

	if (error_position > 0)
	{
		//Error occured at error_position
		int index = current_position + (error_position - 1);
		setBit(buffer, index, getBit(buffer, index) ^ 1);
		Decoded_file.corrected_counter++;
	}

	Decoded_file.wrote_counter += CODE_DATA_SIZE;
	Decoded_file.received_counter += CODE_WORD_SIZE;
}

void getDecodedResult(char *output)
{
	sprintf(output, "received: %d bytes\nwrote: %d bytes\ncorrected: %d errors", 
		Decoded_file.received_counter, Decoded_file.wrote_counter, Decoded_file.corrected_counter);
}

void removeCheckBits(char *buffer, char *output, int index)
{
	int i;
	int position_code = index * CODE_WORD_SIZE;
	int position_data = index * CODE_DATA_SIZE;
	int check_bit_counter = 0;

	for (i = 0; i <= CODE_WORD_SIZE; i++, position_code++)
	{
		int x = i + 1;
		
		if (isPowerOfTwo(x))
		{
			check_bit_counter++;
			continue;
		} 
		
		setBit(output, position_data, getBit(buffer, position_code));
		position_data++;
	}


}

void setBit(char *buffer, int position, int bit)
{
	int charPosition = position / BITS_IN_BYTE;
	int positionInChar = ((BITS_IN_BYTE - 1) - (position % BITS_IN_BYTE));
	if (bit == 0)
		buffer[charPosition] &= ~(1 << positionInChar);
	else 
		buffer[charPosition] |= (1 << positionInChar);
}

int getBit(char *buffer, int position)
{
	return buffer[position / BITS_IN_BYTE] >> ((BITS_IN_BYTE - 1) - (position % BITS_IN_BYTE)) & 1;
}