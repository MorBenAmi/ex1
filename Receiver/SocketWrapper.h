#ifndef SOCKET_WRAPPER_HEADER
#define SOCKER_WRAPPER_HEADER

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 63

BOOL initWSA();

void cleanWSA();

BOOL acceptConnection(SOCKET listen_socket, SOCKET* accepted_socket);

BOOL sockListen(int port, int max_connections, SOCKET* listen_sock);

void closeSocket(SOCKET sock);

BOOL receivePacketsFromSocket(SOCKET socket, char* received_message);

BOOL writeToSocket(SOCKET socket, char *message_to_send);

BOOL connectSocket(char *address, int port, SOCKET *socket);

#endif