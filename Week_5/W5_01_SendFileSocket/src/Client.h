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
	SUCCESS = 200,
	NOT_FOUND = 404
};

class CrossPlatformTCPSocket;

class Client{
private:
	int init_result_; 
	int connect_result_; 
	
	
	int port_;	
	
	char * host_;
	
	CrossPlatformTCPSocket * socket_;

public:
	Client();

	int initClient( const char * host, const int port );

	int getFile( const char * file_name );

	~Client();
};

#endif // _CLIENT_H_
/// EOF Client.h