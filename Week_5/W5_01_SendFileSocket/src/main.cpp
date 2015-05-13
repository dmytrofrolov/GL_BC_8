//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Main file for SendFile project.
//

#include "CrossPlatformTCPSocket.h"
#include "Server.h"
#include "Client.h"
#include <cstdio>

#define INVALID_INPUT 2
#define CLIENT 2
#define SERVER 1

int main( int argc, char* argv[] ){

	char * host;
	int port;
	int ch;
		
	if( argc == 4 ){
		ch = atoi( argv[1] );
		host = argv[2];
		port = atoi( argv[3] );
	}
	else {
		printf("Using:\n SendFile <type> <host> <port>\n");
		printf("<type> : 1 for server, 2 for client\n");
		return INVALID_INPUT;
	}

	
	char file_name[ FILE_NAME_SIZE ];

	if(ch == CLIENT )
	{
		printf("File name : \n");
		scanf("%s", file_name );
		Client * client = new Client();
		
		client->initClient( host, port );

		client->getFile( file_name );

		delete client;
	}
	else if( ch == SERVER )
	{
		Server * server = new Server();

		server->initServer( port, 10 );

		server->startServer();

		server->stopServer();
		
		delete server;
	}

	return 0;

}