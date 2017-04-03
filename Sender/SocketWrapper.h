#ifndef SOCKET_WRAPPER_HEADER
#define SOCKER_WRAPPER_HEADER

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

BOOL initWSA();

void cleanWSA();

void closeSocket(SOCKET sock);

void closeSend(SOCKET sock);

void closeRead(SOCKET sock);

BOOL receiveFromSocket(SOCKET socket, char* message);

BOOL readByteFromSocket(SOCKET sock, unsigned char *byte);

BOOL writeByteToSocket(SOCKET sock, unsigned char byte);

BOOL connectSocket(char * address, int port, SOCKET *socket_client);

#endif