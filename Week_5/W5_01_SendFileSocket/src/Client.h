//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Client class.
// 


#ifndef _CLIENT_H_
#define _CLIENT_H_

#define MIN_PORT 1024
#define ZERO_CLIENTS 0
#define LENGTH_OF_CODE 3
#define NOT_FOUND_ERROR 2

enum ReturnCodes{
	SUCCESS_CODE = 200,
	NOT_FOUND_CODE = 404
};

class CrossPlatformTCPSocket;

class Client{
private:
	// handle result of functions
	int init_result_; 
	int connect_result_; 
	
	
	unsigned int port_;	
	
	char * host_;
	
	CrossPlatformTCPSocket * socket_;

public:
	// inititalize vars with init values
	Client();

	// Initialize socket, bind it with port number and mark it to listen for connections
	// @param   [const char *]	host to connect
	// @param   [const unsigned int]	port to connect
	// @return  0 - if successfull, error code otherwise
	int initClient( const char * host, const unsigned int port );


	// Print file on screen or error message if file not found
	// @param 	[const char *] name of file to get
	// @return  0 - if success or error code
	int getFile( char * file_name );

	// Deallocates all memory from port_ and socket_
	~Client();
};

#endif // _CLIENT_H_

/// EOF Client.h