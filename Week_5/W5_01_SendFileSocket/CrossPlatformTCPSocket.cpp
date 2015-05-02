//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Cross-platform TCP Socket class.
// 

#include "CrossPlatformTCPSocket.h"


/////////////////////////////////////////////////////////////////////////////

CrossPlatformTCPSocket::CrossPlatformTCPSocket():
	bind_result_(-1),
	listen_result_(-1),
	close_result_(-1),
	connect_result_(-1),
	io_socket_(-1),
	reply_socket_(-1)
{

	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = INADDR_ANY;
	sockaddr_in_size_ = sizeof(struct sockaddr_in);

}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::initSocket( void ) {

	if ( io_socket_ > 0 ) { 
		close ( io_socket_ );

		#ifdef _WIN32		
			WSACleanup();
		#endif
	}

	#ifdef _WIN32	
		printf("Initialising Winsock...\n");
		if ( WSAStartup(MAKEWORD(2, 2), &wsa_) != 0 ) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			return 1;
		}
	#endif

	printf("Create Socket...\n");

	io_socket_ = socket( AF_INET , SOCK_STREAM , 0 );

	if ( io_socket_ < 0 ) {
		printf("Could not create socket\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::bindSocket( const unsigned int port ) {
	printf("Bind...\n");

	addr_.sin_port = htons( port );

	bind_result_ = bind( io_socket_, 
						 (struct sockaddr *)&(addr_), 
						 sockaddr_in_size_ );

	if ( bind_result_ < 0) {
		printf("Bind failed\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}

	return bind_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::listenSocket( void ){
	
	if( bind_result_ == BIND_SUCCESS ){
		printf("Listen for request...\n");

		listen_result_ = listen(io_socket_ , 3);
	}
	else{
		printf("Socket was not binded, listening is impossible\n");
	}
	return listen_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::connectToSocket( const unsigned int port ){

	addr_.sin_port = htons( port );

	#ifdef _WIN32
		addr_.sin_addr.s_addr = inet_addr("localhost");
	#endif
	
	#ifdef __linux__
		inet_pton(AF_INET, "localhost", &addr_.sin_addr);
	#endif	
	
	memset(&(addr_.sin_zero), 0, 8);

	connect_result_ = connect( io_socket_, (struct sockaddr *)&addr_, sockaddr_in_size_ );

	if ( connect_result_ < 0 ){
		printf("ERROR: Failed to connect to the host!\n");
		return connect_result_;
	}
	else{
		printf("[Client] Connected to server ...ok!\n");
	}
	return connect_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::sendToSocket( char * request ){
	
	int sendSize = send( io_socket_ , request , strlen(request) , 0 );
		
	return sendSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::receiveFromSocket( char * const buffer, unsigned int buffer_size ){
	if( buffer_size > BUFFER_SIZE ) 
		buffer_size = BUFFER_SIZE;

	int receiveSize = recv( io_socket_, buffer, buffer_size, 0 );
	buffer[ receiveSize ] = '\0';
	return receiveSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::acceptReplyConnection( void ){
	if( listen_result_ == LISTEN_SUCCESS && bind_result_ == BIND_SUCCESS ){
		printf("Wait to accept connection...\n");
		
		reply_socket_ = accept( io_socket_ , (struct sockaddr *)&addr_, &sockaddr_in_size_ );
		
		if ( reply_socket_ < 0 ){
			printf("accept failed \n");
			#ifdef _WIN32
				printf("with error : %d\n", WSAGetLastError());
			#endif
		}
		return reply_socket_;

	}else{
		return -1;
	}	
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::receiveReply( char * const buffer, unsigned int buffer_size ){
	if( buffer_size > BUFFER_SIZE ) 
		buffer_size = BUFFER_SIZE;

	int receiveSize = recv( reply_socket_, buffer, buffer_size, 0 );

	buffer[ receiveSize ] = '\0';
	
	return receiveSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::sendReply( char * request ){
	
	int sendSize = send( reply_socket_ , request , strlen(request) , 0 );

	return sendSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::closeReplyConnection( void ){
	if( reply_socket_ > 0 ){
		#ifdef _WIN32
			close_result_ = closesocket( reply_socket_ );
		#endif
		
		#ifdef __linux__
			close_result_ = close( reply_socket_ );
		#endif	
	}
	return close_result_;
}

/////////////////////////////////////////////////////////////////////////////

CrossPlatformTCPSocket::~CrossPlatformTCPSocket(){
	close ( io_socket_ );

	#ifdef _WIN32		
		WSACleanup();
	#endif
}