#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"
#include "ReadNextBlock.h"
#include "TransformHammingBlock.h"
#include "WriteHammingBlockToSocket.h"

#define MAX_FINISH_MESSAGE_LENGTH 1024

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
	char finish_message[MAX_FINISH_MESSAGE_LENGTH];
	char *bytes_received = NULL;
	char *bytes_reconstructed = NULL;
	char *errors_corrected = NULL;


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
		printf("Unable to open file\n");
		return GetLastError();
	}

	while(ReadNextBlock(*file, block) == TRUE)
	{
		TransformHammingBlock(block, hamming_block);
		WriteHammingBlockToSocket(channel_socket, hamming_block);
	}
	closeSend(channel_socket);


	memset(finish_message, '\0', MAX_FINISH_MESSAGE_LENGTH);
	if(receiveFromSocket(channel_socket, finish_message) == FALSE)
	{
		//todo: error while receiving finish message from channel
	}
	closeSocket(channel_socket);

	bytes_received = strtok(finish_message, ",");
	bytes_reconstructed = strtok(NULL, ",");
	errors_corrected = strtok(NULL, ",");
	
	fprintf(stderr, "received: %s bytes\n", bytes_received);
	fprintf(stderr, "reconstructed: %s bytes\n", bytes_reconstructed);
	fprintf(stderr, "corrected: %s errors\n", errors_corrected);

	cleanWSA();
	return 0;
}