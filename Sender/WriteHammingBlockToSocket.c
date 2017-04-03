#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"
#include "WriteHammingBlockToSocket.h"

static char extra_bits = 0;
static int extra_bits_count = 0;

void WriteHammingBlockToSocket(SOCKET channel_socket, unsigned char hamming_block[8])
{
	int i = 0;
	int bytes_to_write = 7;
	unsigned char temp = 0;

	if(extra_bits_count > 0)
		bytes_to_write = 8;

	for(i = 0; i < bytes_to_write; i++)
	{
		temp = hamming_block[i] >> (8-extra_bits_count);
		hamming_block[i] = hamming_block[i] << extra_bits_count;
		hamming_block[i] = hamming_block[i] | extra_bits;
		writeByteToSocket(channel_socket, hamming_block[i]);
		extra_bits = temp;
	}

	if(extra_bits_count == 0)
	{
		extra_bits = hamming_block[7];
		extra_bits_count = 7;
	}
	else
	{
		extra_bits_count--;
	}
}