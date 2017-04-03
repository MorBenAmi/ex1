#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"

unsigned char addErrorToByte(unsigned char byte, double error_prob)
{
	int i = 0;
	unsigned char temp = 0;
	for(i=0;i<8;i++)
	{
		temp=1<<i;
		if((rand()/(RAND_MAX+1))<=error_prob)
			byte=byte^temp;
	}
	return byte;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc, char* argv[])
{
	int reciever_port=0;
	int sender_port=0;
	double error=0;
	int random_seed=0;
	SOCKET listen_socket;
	SOCKET receiver_sock;
	SOCKET sender_sock;

	unsigned char byte = 0;

	if(argc!=4)
	{
		printf("Not enough input arguments");
		return;
	}

	//init random
	srand(random_seed);

	sender_port=atoi(argv[1]);
	reciever_port=atoi(argv[2]);
	error=atof(argv[3]);
	random_seed=atoi(argv[4]);

	//wait for receiver...
	if(sockListen(reciever_port, &listen_socket) == FALSE){
		//todo: error...
	}
	if(acceptConnection(listen_socket, &receiver_sock) == FALSE){
		//todo: error...
	}

	//wait for sender...
	if(sockListen(sender_port, &listen_socket) == FALSE){
		//todo: error...
	}
	if(acceptConnection(listen_socket, &sender_sock) == FALSE){
		//todo: error...
	}


	while(readByteFromSocket(sender_sock, &byte) == TRUE)
	{
		byte = addErrorToByte(byte, error);
		writeByteToSocket(receiver_sock, byte);
	}
	closeSend(receiver_sock);

	while(readByteFromSocket(receiver_sock,&byte) == TRUE)
	{
		writeByteToSocket(sender_sock, byte);
	}
	closeSend(sender_sock);
	return 0;
}

