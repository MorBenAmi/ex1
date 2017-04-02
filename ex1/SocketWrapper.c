#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SocketWrapper.h"
#include "InputHandler.h"

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
	char buffer[2];
	int result = 0;

	shutdown(sock, SD_SEND);

	do
	{
		result = recv(sock, buffer, 1,0);
	} while( result > 0);

	closesocket(sock);
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
		if(WSAGetLastError() == WSAEWOULDBLOCK)
		{
			//no waiting socket.. return true but not socket was accepted!
			WSASetLastError(0);
			return TRUE;
		}
		SetLastError(WSAGetLastError());
		return FALSE;
	}
	return TRUE;
}

//Receive a message from the socket until shutdown recieved
BOOL receivePacketsFromSocket(SOCKET socket, char* buffer)
{
	char* cur_place_ptr = buffer;
	int count;
	int total_received = 0;
	
	while (1)  
	{
		/* send does not guarantee that the entire message is sent */
		count = recv(socket, cur_place_ptr, BUFFER_SIZE, 0);
		while(WSAGetLastError() == WSAEWOULDBLOCK) //when the socket is nonblocking: no data available yet
		{
			WSASetLastError(0);
			Sleep(100);
			count = recv(socket, cur_place_ptr, BUFFER_SIZE, 0);
		}
		if (count == SOCKET_ERROR) 
		{
			SetLastError(WSAGetLastError());
			return FALSE;
		}		
		else if (count == 0) 
			return TRUE; // recv() returns zero if connection was gracefully disconnected.

		total_received += count;
		//handle
		if (total_received >= BUFFER_SIZE)
		{
			decodeMessage(buffer);
			
			cur_place_ptr = buffer;
			total_received = count;
		} else {
			cur_place_ptr += count; // <ISP> pointer arithmetic
		}
	}
	return TRUE;
}

//Writes the message_to_send to the socket
BOOL writeToSocket(SOCKET socket, char *message_to_send)
{
	const char* cur_place_ptr = message_to_send;
	int bytes_transferred;
	int remaining_bytes_to_send = strlen(message_to_send);
	
	while (remaining_bytes_to_send > 0)  
	{
		/* send does not guarantee that the entire message is sent */
		bytes_transferred = send(socket, cur_place_ptr, remaining_bytes_to_send, 0);
		if ( bytes_transferred == SOCKET_ERROR ) 
		{
			SetLastError(WSAGetLastError());
			return FALSE;
		}
		
		remaining_bytes_to_send -= bytes_transferred;
		cur_place_ptr += bytes_transferred; // <ISP> pointer arithmetic
	}

	shutdown(socket, SD_SEND);
	return TRUE;
}

//Connects to local socket to specific port. return TRUE if succeed
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
