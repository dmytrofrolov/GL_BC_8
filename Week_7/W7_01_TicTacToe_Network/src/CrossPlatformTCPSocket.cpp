//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Cross-platform TCP Socket class.
// 

#include "CrossPlatformTCPSocket.h"

unsigned int CrossPlatformTCPSocket::socket_number = 0;
#ifdef _WIN32
	int CrossPlatformTCPSocket::wsa_start_result_ = ERROR_RESULT;
#endif
/////////////////////////////////////////////////////////////////////////////

CrossPlatformTCPSocket::CrossPlatformTCPSocket():
	bind_result_( ERROR_RESULT ),
	listen_result_( ERROR_RESULT ),
	close_result_( ERROR_RESULT ),
	connect_result_( ERROR_RESULT ),
	io_socket_( ERROR_RESULT ),
	reply_socket_( ERROR_RESULT )
{
	// if there is not objects created
	if( socket_number == 0 ){
		#ifdef _WIN32	
			printf("[SOCKET] Initialising Winsock...\n");

			wsa_start_result_ = WSAStartup( WSA_VERSION, &wsa_ );

			if ( wsa_start_result_ != SUCCESS_RESULT ) {
				printf("Failed. Error Code : %d", WSAGetLastError());
			}
		#endif
	}

	++socket_number;


	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = INADDR_ANY;
	sockaddr_in_size_ = sizeof(struct sockaddr_in);

}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::initSocket( void ) {

	// if this CrossPlatformTCPSocket object was in use before
	if( (int)io_socket_ > 0 ){
		closeSocket();
	}

	#ifdef _WIN32	
		if ( wsa_start_result_ != SUCCESS_RESULT ) {
			return SOCKET_INIT_ERROR;
		}
	#endif

	printf("[SOCKET] Create Socket...\n");

	io_socket_ = socket( AF_INET, SOCK_STREAM, 0 );

	if ( io_socket_ < 0 ) {
		printf("[SOCKET] Could not create socket\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}

	return SUCCESS_RESULT;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::initSocket( const int socket ) {

	if( socket < 0 ){
		return INVALID_SOCKET_ERROR;
	}

	// if this CrossPlatformTCPSocket object was in use before
	if( (int)io_socket_ > 0 ){
		closeSocket();
	}

	#ifdef _WIN32	
		if ( wsa_start_result_ != SUCCESS_RESULT ) {
			printf("%d\n", wsa_start_result_ );
			return SOCKET_INIT_ERROR;
		}
	#endif

	printf("[SOCKET] Create Socket...\n");

	io_socket_ = socket;

	if ( io_socket_ < 0 ) {
		printf("[SOCKET] Could not create socket\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
		return INVALID_SOCKET_ERROR;
	}

	return SUCCESS_RESULT;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::bindSocket( const unsigned int port ) {

	if( (int)io_socket_ < 0 ){
		return ERROR_RESULT;
	}

	if( bind_result_ != ERROR_RESULT ) {
		printf("[SOCKET] Already binded\n");
		return bind_result_;
	}

	printf("[SOCKET] Bind...\n");

	addr_.sin_port = htons( port );

	bind_result_ = bind( io_socket_, (struct sockaddr *)&(addr_), sockaddr_in_size_ );

	if ( bind_result_ < 0) {
		printf( "[SOCKET] Bind failed\n" );
		#ifdef _WIN32
			printf( "with error code : %d\n", WSAGetLastError() );
		#endif
	}

	return bind_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::listenSocket( const unsigned int max_clients_queue ){
	
	if( listen_result_ == LISTEN_SUCCESS ){
		printf("[SOCKET] Already listening\n");
		return listen_result_;
	}

	if( bind_result_ == BIND_SUCCESS && (int)io_socket_ > 0 ){
		printf("[SOCKET] Listen for request...\n");

		listen_result_ = listen( io_socket_, max_clients_queue );
	}
	else{
		printf("[SOCKET] Socket was not binded, listening is impossible\n");
	}
	return listen_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::connectToSocket( char * host, const unsigned int port ){

	if( host == NULL || (int)io_socket_ < 0 ){
		return connect_result_;
	}

	addr_.sin_port = htons( port );

	#ifdef _WIN32
		addr_.sin_addr.s_addr = inet_addr( host );
	#endif
	
	#ifdef __linux__
		inet_pton(AF_INET, host, &addr_.sin_addr);
	#endif	
	
	memset(&(addr_.sin_zero), 0, sizeof(addr_.sin_zero) ); // 8

	connect_result_ = connect( io_socket_, (struct sockaddr *)&addr_, sockaddr_in_size_ );

	if ( connect_result_ < 0 ){
		printf("[SOCKET] ERROR: Failed to connect to the host!\n");
		#ifdef _WIN32
			printf("with error code : %d\n", WSAGetLastError());
		#endif
	}
	else{
		printf("[SOCKET] Connected to server ...ok!\n");
	}
	return connect_result_;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::sendToSocket( const char * request ){
	if( request == NULL || (int)io_socket_ < 0 ){
		return ERROR_RESULT;
	}

	int sendSize = send( io_socket_, request, strlen(request), 0 );
		
	return sendSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::receiveFromSocket( char * const buffer, const size_t buffer_size ){
	
	if( buffer == NULL || buffer_size == 0 || (int)io_socket_ < 0 ){
		return ERROR_RESULT;
	}

	// set timeout for socket
	
	struct timeval tv;

	tv.tv_sec = RECEIVE_TIMEOUT;  /* 30 Secs Timeout */
	tv.tv_usec = 0;  // Not init'ing this can cause strange errors

	setsockopt(io_socket_, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

	memset( buffer, 0, buffer_size );
	int receiveSize = recv( io_socket_, buffer, buffer_size, 0 );
	if( receiveSize > 0 ){
		buffer[ receiveSize ] = '\0';
	}
	return receiveSize;
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::acceptReplyConnection( void ){

	if( listen_result_ == LISTEN_SUCCESS && bind_result_ == BIND_SUCCESS && (int)io_socket_ > 0 ){
		printf("[SOCKET] Wait to accept connection...\n");

		// turn off timeout for socket because it can listen for a long time
		struct timeval tv;

		tv.tv_sec = 0;  /* 0 Secs Timeout */
		tv.tv_usec = 0;  // Not init'ing this can cause strange errors

		setsockopt(io_socket_, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));


		reply_socket_ = accept( io_socket_ , (struct sockaddr *)&addr_, &sockaddr_in_size_ );
		
		if ( reply_socket_ < 0 ){
			printf("[SOCKET] accept failed \n");
			#ifdef _WIN32
				printf("with error : %d\n", WSAGetLastError());
			#endif
		}
		return reply_socket_;

	}else{
		return ERROR_RESULT;
	}	
}

/////////////////////////////////////////////////////////////////////////////

int CrossPlatformTCPSocket::closeSocket( void ){

	if( (int)io_socket_ > 0 ){

		#ifdef _WIN32
			close_result_ = closesocket( io_socket_ );
			if( socket_number == 0 ){	// if there are no Socket object left
				WSACleanup();
			}
		#endif
		
		#ifdef __linux__
			close_result_ = close( io_socket_ );
		#endif	
	}else{
		close_result_ = SUCCESS_RESULT;
	}
	return close_result_;
}

/////////////////////////////////////////////////////////////////////////////

CrossPlatformTCPSocket::~CrossPlatformTCPSocket(){
	--socket_number;
	closeSocket();
}


/// EOF CrossPlatformTCPSocket.cpp