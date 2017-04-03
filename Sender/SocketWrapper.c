#include "SocketWrapper.h"

BOOL init_WSA()
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

void clean_WSA()
{
	WSACleanup();
}

//todo: change this!
void closeSocket(SOCKET sock)
{
	char buffer[2];
	int result = 0;

	shutdown(sock, SD_SEND);

	do
	{
		result = recv(sock, buffer, 1,0);
	} while( result > 0);

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

BOOL connectSocket(int port, char* ip, SOCKET *socket_client)
{
	SOCKADDR_IN clientService;
	
	*socket_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(*socket_client == INVALID_SOCKET)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(port);
	clientService.sin_addr.s_addr = inet_addr(ip);

	if (connect(*socket_client, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) 
	{
        SetLastError(WSAGetLastError());
		WSACleanup();
        return FALSE;
    }

	return TRUE;
}
