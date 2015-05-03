#include "CrossPlatformTCPSocket.h"
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
		CrossPlatformTCPSocket * client = new CrossPlatformTCPSocket();
		
		client->initSocket();
		client->connectToSocket( port );
		client->sendToSocket((char*)"123432");
		client->receiveFromSocket( buffer, 512 );
		printf("%s\n", buffer ); 		
		
		delete client;
	}
	else
	{
		CrossPlatformTCPSocket * server = new CrossPlatformTCPSocket();
		
		server->initSocket();
		server->bindSocket( port );
		server->listenSocket();
		int reply_socket_id;
		CrossPlatformTCPSocket * reply_socket;
		
		while(1){
			reply_socket_id = server->acceptReplyConnection();

			reply_socket = new CrossPlatformTCPSocket();
			
			reply_socket->initSocket( reply_socket_id );
			
			cout << reply_socket->receiveFromSocket( buffer, 512 ) << endl;
			
			reply_socket->sendToSocket(buffer);
			
			delete reply_socket;
		}
		
		delete server;
	}


	system("pause");
	return 0;

}