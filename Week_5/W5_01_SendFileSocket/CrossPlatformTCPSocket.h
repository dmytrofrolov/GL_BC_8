#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef _WIN32

#include <io.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#endif

#ifdef __linux__

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

#define BUFFER_SIZE 512
// 

#ifndef _CROSSPLATFORMTCPSOCKET_H_
#define _CROSSPLATFORMTCPSOCKET_H_

class CrossPlatformTCPSocket {
private:

	struct sockaddr_in addr;
	size_t sockaddr_in_size;
	char buffer[ BUFFER_SIZE ];

	#ifdef _WIN32
		WSADATA wsa;
		SOCKET io_socket, reply_socket;
	#endif

	#ifdef __linux__
		int io_socket, reply_socket;
	#endif

public:
	int initSocket(int port);
	int bindSocket();
	int acceptConnection(int);

	int connectToSocket(int port);
	int closeCurrentConnection(int);

	int listenForRequest(int);

	char * receiveFromSocket(int);

	int sendToSocket( char * );

	CrossPlatformTCPSocket();

	~CrossPlatformTCPSocket();
};

#endif /* _CROSSPLATFORMTCPSOCKET_H_ */