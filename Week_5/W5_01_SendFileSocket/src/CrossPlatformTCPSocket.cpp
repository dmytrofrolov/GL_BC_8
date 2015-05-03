//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Cross-platform TCP Socket class.
// 

#include "CrossPlatformTCPSocket.h"


/////////////////////////////////////////////////////////////////////////////

CrossPlatformTCPSocket::CrossPlatformTCPSocket():
	bind_result_( ERROR_RESULT ),
	listen_result_( ERROR_RESULT ),
	close_result_( ERROR_RESULT ),
	connect_result_( ERROR_RESULT ),
	io_socket_( ERROR_RESULT ),
	reply_socket_( ERROR_RESULT )
{

	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = INADDR_ANY;
	sockaddr_in_size_ = sizeof(struct sockaddr_in);

}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::initSocket( void ) {

	// if this CrossPlatformTCPSocket object was in use before
	if ( io_socket_ > 0 ) { 
		close ( io_socket_ );

		#ifdef _WIN32		
			WSACleanup();
		#endif
	}

	#ifdef _WIN32	
		printf("[SOCKET] Initialising Winsock...\n");
		if ( WSAStartup(MAKEWORD(2, 2), &wsa_) != 0 ) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			return 1;
		}
	#endif

	printf("[SOCKET] Create Socket...\n");

	io_socket_ = socket( AF_INET , SOCK_STREAM , 0 );

	if ( io_socket_ < 0 ) {
		printf("[SOCKET] Could not create socket\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::initSocket( const int socket ) {

	#ifdef _WIN32	
		printf("[SOCKET] Initialising Winsock...\n");
		if ( WSAStartup(MAKEWORD(2, 2), &wsa_) != 0 ) {
			printf("[SOCKET] Failed. Error Code : %d", WSAGetLastError());
			return 1;
		}
	#endif

	printf("[SOCKET] Create Socket...\n");

	io_socket_ = socket;

	if ( io_socket_ < 0 ) {
		printf("[SOCKET] Could not create socket\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
		return 2;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::bindSocket( const unsigned int port ) {
	printf("[SOCKET] Bind...\n");

	addr_.sin_port = htons( port );

	bind_result_ = bind( io_socket_, (struct sockaddr *)&(addr_), sockaddr_in_size_ );

	if ( bind_result_ < 0) {
		printf("[SOCKET] Bind failed\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}

	return bind_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::listenSocket( void ){
	
	if( bind_result_ == BIND_SUCCESS ){
		printf("[SOCKET] Listen for request...\n");

		listen_result_ = listen(io_socket_ , 3);
	}
	else{
		printf("[SOCKET] Socket was not binded, listening is impossible\n");
	}
	return listen_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::connectToSocket( const unsigned int port ){

	addr_.sin_port = htons( port );

	#ifdef _WIN32
		addr_.sin_addr.s_addr = inet_addr("192.168.43.236");
	#endif
	
	#ifdef __linux__
		inet_pton(AF_INET, "192.168.43.236", &addr_.sin_addr);
	#endif	
	
	memset(&(addr_.sin_zero), 0, 8);

	connect_result_ = connect( io_socket_, (struct sockaddr *)&addr_, sockaddr_in_size_ );

	if ( connect_result_ < 0 ){
		printf("[SOCKET] ERROR: Failed to connect to the host!\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}
	else{
		printf("[Client] Connected to server ...ok!\n");
	}
	return connect_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::sendToSocket( char * request ){
	if( request == NULL ){
		return -1;
	}

	int sendSize = send( io_socket_ , request , strlen(request) , 0 );
		
	return sendSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::receiveFromSocket( char * const buffer, const size_t buffer_size ){
	if( buffer == NULL || buffer_size == 0 ){
		return -1;
	}

	memset(buffer, 0, buffer_size);
	int receiveSize = recv( io_socket_, buffer, buffer_size, 0 );
	buffer[ receiveSize ] = '\0';
	return receiveSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::acceptReplyConnection( void ){
	if( listen_result_ == LISTEN_SUCCESS && bind_result_ == BIND_SUCCESS ){
		printf("[SOCKET] Wait to accept connection...\n");
		
		reply_socket_ = accept( io_socket_ , (struct sockaddr *)&addr_, &sockaddr_in_size_ );
		
		if ( reply_socket_ < 0 ){
			printf("[SOCKET] accept failed \n");
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

CrossPlatformTCPSocket::~CrossPlatformTCPSocket(){
	if( io_socket_ > 0 ){
		#ifdef _WIN32
			close_result_ = closesocket( io_socket_ );
			WSACleanup();
		#endif
		
		#ifdef __linux__
			close_result_ = close( io_socket_ );
		#endif	
	}

}


/// EOF CrossPlatformTCPSocket.cpp