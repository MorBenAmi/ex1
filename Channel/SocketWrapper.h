#ifndef SOCKET_WRAPPER_HEADER
#define SOCKER_WRAPPER_HEADER

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

BOOL init_WSA();

void clean_WSA();

void close_socket(SOCKET sock);

void closeSend(SOCKET sock);

void closeRead(SOCKET sock);

BOOL readByteFromSocket(SOCKET sock, unsigned char *byte);

BOOL writeByteToSocket(SOCKET sock, unsigned char byte);

BOOL connectSocket(int port, char* ip, SOCKET* socket);

BOOL acceptConnection(SOCKET listen_sock, SOCKET* accepted_sock);

BOOL sockListen(int port, SOCKET *listen_sock);

#endif