#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "SocketWrapper.h"

unsigned char addErrorToByte(unsigned char byte, double error_prob, int *num_of_changes)
{
	int i = 0;
	unsigned char temp = 0;
	*num_of_changes = 0;
	for(i=0;i<8;i++)
	{
		temp=1<<i;
		if(((double)rand()/(RAND_MAX+1))<=error_prob)
		{
			byte=byte^temp;
			*num_of_changes = (*num_of_changes) + 1;
		}
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
	int num_of_changes = 0;
	int total_changes = 0;
	long total_bytes_received = 0;
	struct sockaddr sender_addr;
	char* sender_ip = NULL;
	struct sockaddr receiver_addr;
	char* receiver_ip = NULL;
	unsigned char byte = 0;

	if(argc!=5)
	{
		printf("%d", argc);
		printf("Not enough input arguments");
		return;
	}


	if(initWSA() == FALSE)
	{
		//todo: error...
	}

	sender_port=atoi(argv[1]);
	reciever_port=atoi(argv[2]);
	error=atof(argv[3]);
	random_seed=atoi(argv[4]);

	//init random
	srand(random_seed);

	//wait for receiver...
	if(sockListen(reciever_port, &listen_socket) == FALSE){
		fprintf(stderr, "Failed to listen to Receiver on port %d, error_code: 0x%x\n", reciever_port, GetLastError());
		return GetLastError();
	}
	if(acceptConnection(listen_socket, &receiver_sock) == FALSE){
		fprintf(stderr, "Failed to accept Receiver's socket, error_code: 0x%x\n", reciever_port, GetLastError());
		return GetLastError();
	}
	closeSocket(listen_socket);

	//wait for sender...
	if(sockListen(sender_port, &listen_socket) == FALSE){
		fprintf(stderr, "Failed to listen to Sender on port %d, error_code: 0x%x\n", sender_port, GetLastError());
		return GetLastError();
	}
	if(acceptConnection(listen_socket, &sender_sock) == FALSE){
		fprintf(stderr, "Failed to accept Sender's socket, error_code: 0x%x\n", reciever_port, GetLastError());
		return GetLastError();
	}
	closeSocket(listen_socket);

	getpeername(sender_sock, &sender_addr, NULL);
	sender_ip = inet_ntoa(((struct sockaddr_in*)&sender_addr)->sin_addr);

	getpeername(receiver_sock, &receiver_addr, NULL);
	receiver_ip = inet_ntoa(((struct sockaddr_in*)&receiver_addr)->sin_addr);

	while(readByteFromSocket(sender_sock, &byte) == TRUE)
	{
		total_bytes_received++;
		byte = addErrorToByte(byte, error, &num_of_changes);
		total_changes += num_of_changes;
		writeByteToSocket(receiver_sock, byte);
	}
	closeSend(receiver_sock);

	while(readByteFromSocket(receiver_sock,&byte) == TRUE)
	{
		writeByteToSocket(sender_sock, byte);
	}
	closeSend(sender_sock);

	closeSocket(receiver_sock);
	closeSocket(sender_sock);
	cleanWSA();

	fprintf(stderr, "sender: %s\n",sender_ip);
	fprintf(stderr, "receiver: %s\n",receiver_ip);
	fprintf(stderr, "%d bytes flipped %d bits\n", total_bytes_received, total_changes);
	return 0;
}

