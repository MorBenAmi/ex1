#define _CRT_SECURE_NO_WARNINGS
#include "HammingUtil.h"
#include <stdio.h>

#define PARITY_SIZE 6

#define isPowerOfTwo(x) (((x) != 0) && !((x) & ((x) - 1)))

void setBit(char *message, int position, int bit);

int getErrorPosition(int *check_bits);

int getBit(char *message, int position);

int getErrorPosition(int *check_bits)
{
	int i;
	int result = 0;

	for (i = 0; i < PARITY_SIZE; i++)
		result += (check_bits[i] * powerTwo(i));

	return result;
}

int hammingDecoder(int current_position, char *message)
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
				check_bits[i] ^= getBit(message, current_position + (j - 1));
		}
	}
	error_position = getErrorPosition(check_bits);

	if (error_position > 0)
	{
		//Error occured at error_position
		int index = current_position + (error_position - 1);
		setBit(message, index, getBit(message, index) ^ 1);
	}

	return error_position;
}

void removeCheckBits(char *message, char *output, int index)
{
	int i;
	int position_code = index * CODE_WORD_SIZE;
	int position_data = index * CODE_DATA_SIZE;

	for (i = 0; i < CODE_WORD_SIZE; i++, position_code++) 
	{
		if (isPowerOfTwo((i + 1)))
		{
			continue;
		} 
		
		setBit(output, position_data, getBit(message, position_code));
		position_data++;
	}
}

void setBit(char *message, int position, int bit)
{
	int charPosition = position / BITS_IN_BYTE;
	int positionInChar = (position % BITS_IN_BYTE);
	if (bit == 0)
		message[charPosition] &= ~(1 << positionInChar);
	else 
		message[charPosition] |= (1 << positionInChar);
}

int getBit(char *message, int position)
{
	return message[position / BITS_IN_BYTE] >> (position % BITS_IN_BYTE) & 1;
}