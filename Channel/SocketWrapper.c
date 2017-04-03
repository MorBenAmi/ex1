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

BOOL sockListen(int port, SOCKET *listen_sock)
{
	SOCKADDR_IN listen_addr;
	u_long nonblockingMode = 1;
	int iResult = 0;
	
	*listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(*listen_sock == INVALID_SOCKET)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(port);
	listen_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(*listen_sock, (SOCKADDR*)&listen_addr, sizeof(listen_addr)) == SOCKET_ERROR)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	if(listen(*listen_sock,0) == SOCKET_ERROR)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL acceptConnection(SOCKET listen_socket, SOCKET* accepted_socket)
{
	struct sockaddr_in connect_socket_addr;
	int addr_len;
	*accepted_socket = INVALID_SOCKET;

	addr_len = sizeof(connect_socket_addr);
	*accepted_socket = accept(listen_socket, (struct sockaddr*)&connect_socket_addr, &addr_len);
	if(*accepted_socket == INVALID_SOCKET)
	{
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}
