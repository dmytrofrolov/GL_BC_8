//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Server class.
// 


#include "Server.h"
#include "CrossPlatformTCPSocket.h"
#include <stdio.h>
#include <string.h>


/////////////////////////////////////////////////////////////////////////////

Server::Server():
	init_result_( ERROR_RESULT ),
	bind_result_( ERROR_RESULT ),
	listen_result_( ERROR_RESULT ),
	stop_result_( ERROR_RESULT ),
	port_( ERROR_RESULT ),
	max_clients_queue_( ERROR_RESULT ),
	server_status_( ERROR_RESULT ),
	socket_( NULL )

{

}

/////////////////////////////////////////////////////////////////////////////

int Server::initServer( const unsigned int port, const unsigned int max_clients_queue ){
	// if server was already started, close it
	if( socket_ != NULL ){
		stop_result_ = stopServer();
		if( stop_result_ != SUCCESS_RESULT ){
			return STOP_ERROR;
		}
		socket_ = NULL;
	}

	// if port is from OS range
	if( port < MIN_PORT ){
		server_status_ = INVALID_PORT_ERROR;
		return INVALID_PORT_ERROR;
	}

	// if user want to server create zero client queue
	if( max_clients_queue == ZERO_CLIENTS ){
		server_status_= ERROR_RESULT;
		return ERROR_RESULT;
	}


	socket_ = new CrossPlatformTCPSocket();
	
	if( socket_ == NULL ) {
		server_status_ = SOCKET_CREATE_ERROR;
		return SOCKET_CREATE_ERROR;
	}

	init_result_ = socket_->initSocket();

	if( init_result_ != SUCCESS_RESULT ) {
		server_status_ = INIT_ERROR;
		return INIT_ERROR;
	}

	bind_result_ = socket_->bindSocket( port );
	
	if( bind_result_ != SUCCESS_RESULT ) {
		server_status_ = BIND_ERROR;
		return BIND_ERROR;
	}

	listen_result_ = socket_->listenSocket( max_clients_queue );

	if( listen_result_ != SUCCESS_RESULT ) {
		server_status_ = LISTEN_ERROR;
		return LISTEN_ERROR;
	}

	server_status_ = SERVER_INITED;
	
	return SUCCESS_RESULT;

}

/////////////////////////////////////////////////////////////////////////////

int Server::startServer( void ){
	if( server_status_ != SERVER_INITED ){
		return START_ERROR;
	}
	
	CrossPlatformTCPSocket * reply_socket = NULL;

	int reply_socket_id = ERROR_RESULT;
	int init_result = SUCCESS_RESULT;
	int bytes_received = 0;
	int bytes_replied = 0;
	
	char buffer[ BUFFER_SIZE ];

	char file_name[ FILE_NAME_SIZE ];
	
	char path_to_file[ BUFFER_SIZE ] = "..\\";

	while( init_result == SUCCESS_RESULT ){
		
		reply_socket_id = socket_->acceptReplyConnection();

		if( reply_socket_id < 0 ){
			break;
		}

		reply_socket = new CrossPlatformTCPSocket();
		
		if( reply_socket == NULL ){
			return SOCKET_CREATE_ERROR;
		}

		init_result = reply_socket->initSocket( reply_socket_id );
			
		if( init_result != SUCCESS_RESULT ){
			return INIT_ERROR;
		}

		bytes_received =  reply_socket->receiveFromSocket( file_name, FILE_NAME_SIZE );

		void * file_end_line = strchr( file_name, '\n' );
		
		if( file_end_line != NULL )
			memset( file_end_line, 0, 1 );

		printf( "Received : %d bytes, path_to_file : %s\n", bytes_received, path_to_file );
		
		// int open_result;
		// File file;
		// open_result = file.openFile( path_to_file, "r" );
		// if( open_result == SUCCESS_RESULT ) {

			bytes_replied += reply_socket->sendToSocket( "200" );

			while ( !feof (pFile) ){
				if ( fgets (buffer, BUFFER_SIZE, pFile) == NULL ) 
					break;

				bytes_replied += reply_socket->sendToSocket( buffer );

				memset( buffer, 0, BUFFER_SIZE );
			}
			// file.closeFile();
		}
		else{

			bytes_replied += reply_socket->sendToSocket( "404" );

		}
		
		bytes_replied += reply_socket->sendToSocket( (char*)"\n\r\n\r" );	
		printf("Bytes replied : %d\n", bytes_replied );
		bytes_replied = 0;	
		delete reply_socket;
		reply_socket = NULL;
	}

	return SUCCESS_RESULT;
}

/////////////////////////////////////////////////////////////////////////////

int Server::stopServer( void ){
	if( socket_ != NULL ){
		delete socket_;
	}

	socket_ = NULL;

	return SUCCESS_RESULT;
}

/////////////////////////////////////////////////////////////////////////////

Server::~Server(){
	if( socket_ != NULL ){

		stop_result_ = stopServer();

	}
}

/// EOF Server.cpp