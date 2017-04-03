#include <stdio.h>
#include "ReadNextBlock.h"

static char extra_bits = 0;
static int extra_bits_count = 0;

BOOL ReadNextBlock(FILE file, unsigned char block[8])
{
	int i = 0;
	int bytes_to_read = 8;
	unsigned char temp = 0;
	unsigned char next_char = 0;

	if(extra_bits_count > 0)
		bytes_to_read = 7;

	for(i = 0; i < bytes_to_read; i++)
	{
		next_char = fgetc(&file);
		if(next_char == EOF) 
			return FALSE;

		block[i] = next_char;
		temp = block[i] >> (8-extra_bits_count);
		block[i] = block[i] << extra_bits_count;
		block[i] = block[i] | extra_bits;
		extra_bits = temp;
	}

	if(extra_bits_count == 0)
	{
		extra_bits = block[7] >> 1;
		block[7] = block[7] & 1;
		extra_bits_count = 7;
	}
	else
	{
		block[7] = extra_bits & 1;
		extra_bits = extra_bits >> 1;
		extra_bits_count--;
	}
	return TRUE;
}