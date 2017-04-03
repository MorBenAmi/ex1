/*Limor Mendelzburg 308081389
Mor Ben Ami 203607536
Exercise 1*/
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"
#include "OutputFileHandler.h"
#include "InputHandler.h"

#define RESULT_LENGTH 300
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc, char* argv[])
{
	char* address = NULL;
	int port = 0;
	SOCKET socket = NULL;
	char buffer[BUFFER_SIZE];
	char *output_path = NULL;
	char decryption_result[RESULT_LENGTH];
	
	address = argv[1];
	port = atoi(argv[2]);
	output_path = argv[3];

	openFile(output_path);

	if (initWSA() == FALSE)
		printf("Failed to init WSA, Error_Code: 0x%x\n", GetLastError());
	if (connectSocket(address, port, &socket) == FALSE)
	{
		printf("Failed connecting to socket, Error_Code: 0x%x\n", GetLastError());
		exit(GetLastError());
	}
	if (receivePacketsFromSocket(socket, buffer) == FALSE)
	{
		printf("Failed receiving from socket, Error_Code: 0x%x\n", GetLastError());
		SetLastError(WSAGetLastError());
		return GetLastError();
	}
	else 
	{
		//Finished reading from socket. Replying with result.
		getDecodedResult(decryption_result);
		fprintf(stderr, "%s", decryption_result);
		if (writeToSocket(socket, decryption_result) == FALSE)
			printf("Failed sending to socket, Error_Code: 0x%x\n", GetLastError());
	}

	//Finished handling files
	closeFile();
	closeSocket(socket);
	cleanWSA();
	
	exit(GetLastError());
}//main1