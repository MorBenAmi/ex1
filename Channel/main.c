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
		if(((double)rand()/(RAND_MAX+1))<error_prob)
		{
			byte=byte^temp;
			*num_of_changes = (*num_of_changes) + 1;
		}
	}
	return byte;
}

BOOL waitForSockets(int sender_port,int reciever_port,SOCKET* sender_sock, SOCKET* receiver_sock)
{
	SOCKET listen_socket;

	//wait for receiver...
	if(sockListen(reciever_port, &listen_socket) == FALSE){
		fprintf(stderr, "Failed to listen to Receiver on port %d, error_code: 0x%x\n", reciever_port, GetLastError());
		return FALSE;
	}
	if(acceptConnection(listen_socket, receiver_sock) == FALSE){
		fprintf(stderr, "Failed to accept Receiver's socket, error_code: 0x%x\n", reciever_port, GetLastError());
		return FALSE;
	}
	closeSocket(listen_socket);

	//wait for sender...
	if(sockListen(sender_port, &listen_socket) == FALSE){
		fprintf(stderr, "Failed to listen to Sender on port %d, error_code: 0x%x\n", sender_port, GetLastError());
		return GetLastError();
	}
	if(acceptConnection(listen_socket, sender_sock) == FALSE){
		fprintf(stderr, "Failed to accept Sender's socket, error_code: 0x%x\n", reciever_port, GetLastError());
		return GetLastError();
	}
	closeSocket(listen_socket);

	return TRUE;
}

char* getIp(SOCKET sock)
{
	SOCKADDR_IN sock_addr;
	IN_ADDR sock_in_addr;
	int addr_len = 0;

	addr_len = sizeof(sock_addr);
	getpeername(sock, (LPSOCKADDR)&sock_addr, &addr_len);

	memcpy(&sock_in_addr, &sock_addr.sin_addr.s_addr, 4);
	return inet_ntoa(sock_in_addr);
}

void transferFromSenderToReceiver(SOCKET sender_sock, SOCKET receiver_sock, double error_prob, int* total_bytes_received, int* total_changes)
{
	unsigned char byte = 0;
	int num_of_changes = 0;

	*total_bytes_received = 0;
	*total_changes = 0;
	while(readByteFromSocket(sender_sock, &byte) == TRUE)
	{
		*total_bytes_received = (*total_bytes_received) + 1;
		byte = addErrorToByte(byte, error_prob, &num_of_changes);
		*total_changes = (*total_changes) + num_of_changes;
		writeByteToSocket(receiver_sock, byte);
	}
	closeSend(receiver_sock);
}

void transferResponseToSender(SOCKET sender_sock, SOCKET receiver_sock)
{
	unsigned char byte = 0;
	while(readByteFromSocket(receiver_sock,&byte) == TRUE)
	{
		writeByteToSocket(sender_sock, byte);
	}
	closeSend(sender_sock);
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
int main(int argc, char* argv[])
{
	int reciever_port=0;
	int sender_port=0;
	double error_prob=0;
	int random_seed=0;

	SOCKET receiver_sock;
	SOCKET sender_sock;
	int total_changes = 0;
	int total_bytes_received = 0;

	if(argc!=5)
	{
		printf("%d", argc);
		printf("Not enough input arguments");
		return;
	}


	if(initWSA() == FALSE)
	{
		printf("Error initWSA failed");
		return GetLastError();
	}

	sender_port=atoi(argv[1]);
	reciever_port=atoi(argv[2]);
	error_prob=atof(argv[3]);
	random_seed=atoi(argv[4]);

	//init random
	srand(random_seed);

	if(waitForSockets(sender_port, reciever_port, &sender_sock, &receiver_sock) == FALSE)
	{
		return GetLastError();
	}
	transferFromSenderToReceiver(sender_sock, receiver_sock, error_prob, &total_bytes_received, &total_changes);
	transferResponseToSender(sender_sock, receiver_sock);

	fprintf(stderr, "sender: %s\n",getIp(sender_sock));
	fprintf(stderr, "receiver: %s\n",getIp(receiver_sock));
	fprintf(stderr, "%d bytes flipped %d bits\n", total_bytes_received, total_changes);

	closeSocket(receiver_sock);
	closeSocket(sender_sock);
	cleanWSA();

	return 0;
}

