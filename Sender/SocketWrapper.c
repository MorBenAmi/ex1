#include "SocketWrapper.h"

BOOL initWSA()
{
	WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
	{
		SetLastError(iResult);
        return FALSE;
	}
	return TRUE;
}

void cleanWSA()
{
	WSACleanup();
}

void closeSocket(SOCKET sock)
{
	closesocket(sock);
}

void closeSend(SOCKET sock)
{
	shutdown(sock, SD_SEND);
}

void closeRead(SOCKET sock)
{
	shutdown(sock, SD_RECEIVE);
}

BOOL receiveFromSocket(SOCKET socket, char* message)
{
	char* cur_place_ptr = message;
	int bytes_just_transferred;
	
	while (1)  
	{
		/* send does not guarantee that the entire message is sent */
		bytes_just_transferred = recv(socket, cur_place_ptr, 1, 0);
		if (bytes_just_transferred == SOCKET_ERROR) 
		{
			SetLastError(WSAGetLastError());
			return FALSE;
		}
		else if (bytes_just_transferred == 0) 
			return TRUE; // recv() returns zero if connection was gracefully disconnected.

		cur_place_ptr += bytes_just_transferred; // <ISP> pointer arithmetic
	}
	return TRUE;
}

BOOL readByteFromSocket(SOCKET socket, unsigned char *byte)
{	
	int bytes_received = 0;
	/* send does not guarantee that the entire message is sent */
	bytes_received = recv(socket, byte, 1, 0);
	while(WSAGetLastError() == WSAEWOULDBLOCK) //when the socket is nonblocking: no data available yet
	{
		WSASetLastError(0);
		Sleep(100);
		bytes_received = recv(socket, byte, 1, 0);
	}
	if (bytes_received == SOCKET_ERROR) 
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}		
	else if (bytes_received == 0) 
		return FALSE; // recv() returns zero if connection was gracefully disconnected.

	return TRUE;
}

BOOL writeByteToSocket(SOCKET socket, unsigned char byte)
{
	int bytes_transferred = 0;
	while ( bytes_transferred == 0 )  
	{
		/* send does not guarantee that the entire message is sent */
		bytes_transferred = send(socket, &byte, 1, 0);
		if ( bytes_transferred == SOCKET_ERROR ) 
		{
			SetLastError(WSAGetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

BOOL connectSocket(char * address, int port, SOCKET *socket_client)
{
	SOCKADDR_IN socket_addr;
	
	*socket_client = socket(PF_INET, SOCK_STREAM, 0);
	if(*socket_client == INVALID_SOCKET)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	socket_addr.sin_family = PF_INET;
	socket_addr.sin_port = htons(port);
	socket_addr.sin_addr.s_addr = inet_addr(address);

	if (connect(*socket_client, (SOCKADDR*) &socket_addr, sizeof(socket_addr) ) == SOCKET_ERROR)
	{
        SetLastError(WSAGetLastError());
		WSACleanup();
        return FALSE;
    }
	return TRUE;

}