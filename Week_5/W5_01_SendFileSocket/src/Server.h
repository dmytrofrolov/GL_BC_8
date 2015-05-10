//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Server class.
// 


#ifndef _SERVER_H_
#define _SERVER_H_

#define MIN_PORT 1024
#define ZERO_CLIENTS 0

class CrossPlatformTCPSocket;


enum SERVER_STATUS {
	SERVER_STARTED = 0,
	SERVER_STOPPED
};

class Server{

private:
	int init_result_; 
	int bind_result_;
	int listen_result_;
	int stop_result_;
	int port_;
	int max_clients_queue_;
	int server_status_;
	CrossPlatformTCPSocket * socket_;

public:
	Server();

	// Initialize socket, bind it with port number and mark it to listen for connections
	// @param   [const unsigned int]	port to bind
	// @param   [const unsigned int]	max number of clients in queue for connect
	// @return  0 - if successfull, error code otherwise
	int initServer( const unsigned int port, const unsigned int max_clients_queue );

	// Start server with connection listening
	// @return error code
	int startServer( void );

	// Deletes socket with connection close, mark server as closed
	// @return 0 if success or error code
	int stopServer( void );
	
	~Server();
};

#endif // _SERVER_H_

/// EOF Server.h