//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Cross-platform TCP Socket class.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32

#include <io.h>
#include <winsock2.h>

#endif

#ifdef __linux__

#include <unistd.h>		// accept(), socket(), close(), etc.
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>	// inet_pton()

#endif

#define BUFFER_SIZE 512
#define BIND_SUCCESS 0
#define LISTEN_SUCCESS 0
#define ERROR_RESULT -1
#define INVALID_SOCKET_ERROR -2
#define SOCKET_INIT_ERROR 1
#define WSA_VERSION MAKEWORD(2, 2)
#define MIN_PORT 1024
#define RECEIVE_TIMEOUT 30

enum SOCKET_ERRORS {
	SUCCESS_RESULT = 0,
	SOCKET_CREATE_ERROR,
	INIT_ERROR,
	LISTEN_ERROR,
	BIND_ERROR,
	CONNECT_ERROR,
	START_ERROR,
	STOP_ERROR,
	INVALID_PORT_ERROR,
	INVALID_HOST_ERROR,
	GET_FILE_ERROR,
	SEND_FILE_ERROR
};

#ifndef _CROSSPLATFORMTCPSOCKET_H_
#define _CROSSPLATFORMTCPSOCKET_H_

class CrossPlatformTCPSocket {

private:

	struct sockaddr_in addr_;
	int bind_result_, listen_result_, close_result_, connect_result_;

	#ifdef _WIN32
		int sockaddr_in_size_;
		static int wsa_start_result_;
		WSADATA wsa_;
		SOCKET io_socket_, reply_socket_;
	#endif

	#ifdef __linux__
		size_t sockaddr_in_size_;
		int io_socket_, reply_socket_;
	#endif

public:

	// number of used sockets
	static unsigned int socket_number;

	// ctor with vars initialization
	CrossPlatformTCPSocket();

	// Initialize socket for work
	// @return  0 - if init successfull
	int initSocket( void );


	// Initialize socket with another socket e.g. with accept reply connection for server
	// @param [const int] - valid socket id e.g. from acceptReplyConnection()
	// @return  0 - if init successfull, error code otherwise
	int initSocket( const int socket );


	// Bind socket to port for listening, if not successfull
	// @param   [const unsigned int]	port to bind
	// @return 0 - if bind successfull, -1 if error
	int bindSocket( const unsigned int port );


	// mark socket as passive listener in port binded before
	// @param   [const unsigned int]	max number of clients in queue for connect
	// @return 0 if success or -1 if this error 
	int listenSocket( const unsigned int max_clients_queue );


	// initiate a connection on a socket at port
	// @param   [const unsigned int]	port to connect
	// @param   [char*]	host to connect
	// @return 0 if success or -1 if error
	int connectToSocket( char * host, const unsigned int port );


	// write to reply_socket_
	// @param [const char*] - pointer to message to send
	// @return - number of bytes sended, -1 if error
	int sendToSocket( const char * request );


	// read from io_socket_
	// @param [char * const] - pointer to buffer
	// @param [unsigned int] - size of buffer
	// @return size what received or -1 if error
	int receiveFromSocket( char * const buffer, const size_t buffer_size );


	// accept input connection ( if is binded and listening ) to reply_socket_
	// @return socket id if success or -1 if error
	int acceptReplyConnection( void );

	// close io_socket_
	// @return 0 if success or error code if error
	int closeSocket( void );

	// dtor
	~CrossPlatformTCPSocket();
};

#endif /* _CROSSPLATFORMTCPSOCKET_H_ */