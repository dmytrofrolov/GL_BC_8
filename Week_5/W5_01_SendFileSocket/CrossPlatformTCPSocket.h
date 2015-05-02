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
 

#ifndef _CROSSPLATFORMTCPSOCKET_H_
#define _CROSSPLATFORMTCPSOCKET_H_

class CrossPlatformTCPSocket {

private:

	struct sockaddr_in addr_;
	char buffer_[ BUFFER_SIZE ];
	int bind_result_, listen_result_, close_result_, connect_result_;

	#ifdef _WIN32
		int sockaddr_in_size_;
		WSADATA wsa_;
		SOCKET io_socket_, reply_socket_;
	#endif

	#ifdef __linux__
		size_t sockaddr_in_size_;
		int io_socket_, reply_socket_;
	#endif

public:

	// ctor with vars initialization
	CrossPlatformTCPSocket();

	// Initialize socket for work
	// @return  0 - if init successfull
	int initSocket( void );


	// Bind socket to port for listening, if not successfull
	// @param   [const unsigned int]	port to bind
	// @return 0 - if bind successfull, -1 if error
	int bindSocket( const unsigned int port );


	// mark socket as passive listener in port binded before
	// @return 0 if success or -1 if this error 
	int listenSocket( void );


	// initiate a connection on a socket at port
	// @param   [const unsigned int]	port to connect
	// @return 0 if success or -1 if error
	int connectToSocket( const unsigned int port );


	// write to reply_socket_
	// @param [char*] - pointer to message to send
	// @return - number of bytes sended, -1 if error
	int sendToSocket( char * request );


	// read from io_socket_
	// @param [char * const] - pointer to buffer
	// @param [const unsigned int] - size of buffer
	// @return size what received or -1 if error
	int receiveFromSocket( char * const buffer, unsigned int buffer_size );


	// accept input connection ( if is binded and listening ) to reply_socket_
	// @return socket id if success or -1 if error
	int acceptReplyConnection( void );

	// read from reply_socket_
	// @param [char * const] - pointer to buffer
	// @param [const unsigned int] - size of buffer
	// @return size what received or -1 if error
	int receiveReply( char * const buffer, unsigned int buffer_size );

	// write to reply_socket_
	// @param [char*] - pointer to message to send
	// @return - number of bytes sended, -1 if error
	int sendReply( char * request );

	// close reply_socket_
	// @return 0 if success or -1 if error		
	int closeReplyConnection( void );

	// dtor
	~CrossPlatformTCPSocket();
};

#endif /* _CROSSPLATFORMTCPSOCKET_H_ */