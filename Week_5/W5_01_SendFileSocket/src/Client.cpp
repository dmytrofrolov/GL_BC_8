//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Client class.
// 


#include "Client.h"
#include "CrossPlatformTCPSocket.h"
#include "File.h"
#include <stdio.h>
#include <string.h>



/////////////////////////////////////////////////////////////////////////////

Client::Client():
	init_result_ ( ERROR_RESULT ),
	connect_result_ ( ERROR_RESULT ),
	port_ ( ERROR_RESULT ),
	host_ ( NULL ),
	socket_ ( NULL )
	// initialize all vars with init vals
{

}

/////////////////////////////////////////////////////////////////////////////

int Client::initClient( const char * host, const unsigned int port ){
	// if host invalid
	if( host == NULL ) {
		return INVALID_HOST_ERROR;
	}

	// if port invalid
	if( port < MIN_PORT ){
		return INVALID_PORT_ERROR;
	}

	// if client was already used ( this is not first init ) 
	if( host_ != NULL && port_ > MIN_PORT ){
		// clear this Client object
		socket_->closeSocket();
		socket_ = NULL;
		delete [] host_;
		host_ = NULL;
		port_ = 0;
	}

	// remember host
	host_ = new char[ strlen( host ) + 1 ];
	strcpy( host_, host );

	// remember port
	port_ = port;

	// alloc memory for socket
	socket_ = new CrossPlatformTCPSocket();

	// if memory was not allocated
	if( socket_ == NULL ){
		return SOCKET_CREATE_ERROR;
	}

	init_result_ = socket_->initSocket();

	// if socket was not initialized successful
	if( init_result_ != SUCCESS_RESULT ){
		return INIT_ERROR;
	}

	// connect to host and port
	connect_result_ = socket_->connectToSocket( host_, port_ );

	// if connections was not successfull
	if( connect_result_ != SUCCESS_RESULT ){
		return CONNECT_ERROR;
	}

	return SUCCESS_RESULT;
}

/////////////////////////////////////////////////////////////////////////////

int Client::getFile( char * file_name ){
	// if it was not connected right
	if( connect_result_ != SUCCESS_RESULT )
		return CONNECT_ERROR;

	// if filename invalid
	if( file_name == NULL ){
		return GET_FILE_ERROR;
	}

	// buffer for getting file from socket
	char buffer[ BUFFER_SIZE ];

	// number of bytes what reveived
	int bytes_received;

	// before sending file, server return code 200 - success, 404 - file not found
	int request_code;

	// send to server file_name
	socket_->sendToSocket( file_name );

	// read from server request_code
	bytes_received = socket_->receiveFromSocket( buffer, LENGTH_OF_CODE );

	if( bytes_received != LENGTH_OF_CODE ){
		printf("%s\n", buffer);
		printf("%d\n", atoi(buffer) );
		return GET_FILE_ERROR;
	}

	// remember request code in int val
	request_code = atoi( buffer );

	// if file found
	if( request_code == SUCCESS_CODE ){
		// read from server file and print it to screen
		File file;
		
		int open_result, write_result;

		open_result = file.openFile( file_name, WRITE_PARAM );
		if( open_result == SUCCESS_RESULT ){
			do{ 
				
				bytes_received = socket_->receiveFromSocket( buffer, BUFFER_SIZE );
				
				if( bytes_received > 0 ){
					write_result = file.writeFile( buffer, bytes_received );
					if( write_result != SUCCESS_RESULT ){
						printf("[CLIENT] Write to file error\n" );
					}
				}else{
					break;
				}


			} while( 1 );
		}else{
			printf("[CLIENT] File can not be open\n");
			return open_result;
		}

		printf("[CLIENT] File received successfully\n" );
		return SUCCESS_RESULT;
	}
	// if file not found
	else if( request_code == NOT_FOUND_CODE ) {
		printf("[CLIENT] File %s not found on server\n", file_name );
		return NOT_FOUND_ERROR;
	}
	// if other error
	else return GET_FILE_ERROR;

	
}

/////////////////////////////////////////////////////////////////////////////

Client::~Client(){
	// if we initialized socket and used it, deallocate all memory
	if( socket_ != NULL ){
		delete socket_;
		socket_ = NULL;
	} 

	if( host_ != NULL ){
		delete [] host_;
		host_ = NULL;
	}
}

/// EOF Client.cpp