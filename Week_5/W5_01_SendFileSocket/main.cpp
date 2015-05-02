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
		
		while(1){
			server->acceptReplyConnection();
			server->receiveReply( buffer, 512 );
			cout << buffer << endl;
			server->sendReply((char*)" sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.12212431\n");
			server->closeReplyConnection();
		}
		
		delete server;
	}

}