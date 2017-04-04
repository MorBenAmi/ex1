/*Limor Mendelzburg 308081389
Mor Ben Ami 203607536
Exercise 1*/
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"
#include "OutputFileHandler.h"
#include "InputHandler.h"

#define RESULT_LENGTH 100
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc, char* argv[])
{
	char* address = NULL;
	int port = 0;
	SOCKET socket = NULL;
	char buffer[BUFFER_SIZE];
	char *output_path = NULL;
	char result_data[RESULT_LENGTH] = { '\0' };

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
		file_decorder decoded_file = getDecodedFile();
			
		sprintf (result_data, "%d,%d,%d", 
			decoded_file.received_counter, 
			decoded_file.wrote_counter, 
			decoded_file.corrected_counter);
		
		//Finished reading from socket. Replying with result.
		fprintf(stderr, "received: %d bytes\nwrote: %d bytes\ncorrected: %d errors", 
			decoded_file.received_counter, decoded_file.wrote_counter, decoded_file.corrected_counter);
		if (writeToSocket(socket, result_data) == FALSE)
			printf("Failed sending to socket, Error_Code: 0x%x\n", GetLastError());
	}

	//Finished handling files
	closeFile();
	closeSocket(socket);
	cleanWSA();
	
	exit(GetLastError());
}//main1