#include "CrossPlatformTCPSocket.h"
#include "Server.h"
#include <cstdio>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main(){


	char ch;
	cout << "Server - 1, Client - 2 : "<< endl;
	cin >> ch;

	cout << "Input port : "<< endl; 
	int port;
	cin >> port;

	char buffer[512];

	if(ch == '2')
	{
		cout << "File name : " << endl;
		cin >> buffer;
		CrossPlatformTCPSocket * client = new CrossPlatformTCPSocket();
		
		client->initSocket();

		client->connectToSocket( (char*)"127.0.0.1", port );

		client->sendToSocket( buffer );

		while( client->receiveFromSocket( buffer, 512 ) > 0)
			printf("%s", buffer ); 		
		
		delete client;
	}
	else
	{
		Server * server = new Server();

		server->initServer( port, 10 );

		server->startServer();

		server->stopServer();
		
		delete server;
	}


	system("pause");
	return 0;

}