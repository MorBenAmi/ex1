/*Limor Mendelzburg 308081389
Mor Ben Ami 203607536
Exercise 4*/
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"
#include "OutputFileHandler.h"
#include "InputHandler.h"


/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc, char* argv[])
{
	char* address = NULL;
	int port = 0;
	SOCKET socket = NULL;
	char buffer[BUFFER_SIZE];
	char *output_path = NULL;
	char decryption_result[1000];

	address = argv[1];
	port = atoi(argv[2]);
	output_path = argv[3];

	openFile(output_path);

	if (initWSA() == FALSE)
		printf("Failed to init WSA, Error_Code: 0x%x\n", GetLastError());
	if (connectSocket(address, port, &socket) == FALSE)
		exit(GetLastError());

	if (receivePacketsFromSocket(socket, buffer) == FALSE)
	{
		SetLastError(WSAGetLastError());
		//write_log_and_print("Error while trying to receive data to socket. Error code: 0x%x\n", GetLastError());
		return GetLastError();
	}
	else 
	{
		//Finished reading from socket. Replying with result.
		getDecryptedResult(decryption_result);
		fprintf(stderr, "%s", decryption_result);
		if (writeToSocket(socket, decryption_result) == FALSE)
		{
			//Failed sending todo
		}
	}

	//Finished handling files
	closeFile();
	closeSocket(socket);
	cleanWSA();
	
	exit(GetLastError());
}//main1