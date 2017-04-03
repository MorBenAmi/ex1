#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"
#include "ReadNextBlock.h"
#include "TransformHammingBlock.h"
#include "WriteHammingBlockToSocket.h"

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc, char* argv[])
{
	char* channel_ip=NULL;
	int port=0;
	char* file_name=NULL;
	FILE* file = NULL;
	unsigned char block[8] = { 0 };
	unsigned char hamming_block[8] = { 0 };
	SOCKET channel_socket;

	//todo: uncomment for test
	/*unsigned char block_test[8] = {139,139,139,139,139,139,139,1};
	unsigned char hamming_block_test[8] = {0};

	TransformHammingBlock(block_test, hamming_block_test);*/

	if(argc!=3)
	{
		printf("Not enough input arguments");
		return;
	}

	channel_ip = argv[1];
	port=atoi(argv[2]);
	file_name=argv[3];

	if(connectSocket(port, channel_ip, &channel_socket) == FALSE)
	{
		printf("Error accured while tring to connect to socket..");
		return GetLastError();
	}

	file=fopen(file_name, "r");
	if (file == NULL)
	{
		printf("Unable to open log file\n");
		exit(GetLastError());
	}

	while(ReadNextBlock(*file, block) == TRUE)
	{
		TransformHammingBlock(block, hamming_block);
		WriteHammingBlockToSocket(channel_socket, hamming_block);
	}
	closeSend(channel_socket);

	//todo: wait for channel response

	//todo: print channel response

	//
	return 0;
}