#include "CrossPlatformTCPSocket.h"
#include "Server.h"
#include "Client.h"
#include <cstdio>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main( int argc, char* argv[] ){
	char * host;
	int port;
	int ch;

	if( argc == 2 && argv[1] == "-h" ){
		printf("Using:\n SendFile <type> <host> <port>\n");
		printf("<type> : 1 for server, 2 for client\n");
	}
	else if( argc == 4 ){
		ch = atoi( argv[1] );
		host = argv[2];
		port = atoi( argv[3] );
	}
	else return 2;

	cout << host << " " << port << " " << ch << endl;

	char buffer[512];

	if(ch == 2 )
	{
		cout << "File name : " << endl;
		cin >> buffer;
		Client * client = new Client();
		
		client->initClient( host, port  );

		client->getFile( buffer );

		delete client;
	}
	else if( ch == 1 )
	{
		Server * server = new Server();

		server->initServer( port, 10 );

		server->startServer();

		server->stopServer();
		
		delete server;
	}

	#ifdef _WIN32
		system("pause");
	#endif
	
	return 0;

}