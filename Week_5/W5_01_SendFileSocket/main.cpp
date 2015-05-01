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
	if(ch == '2')
	{
		CrossPlatformTCPSocket * client = new CrossPlatformTCPSocket();
		client->initSocket(8887);
		client->connectToSocket(0);
		printf("%s\n", client->receiveFromSocket(0) ); 
		delete client;
	}
	else
	{
		CrossPlatformTCPSocket * server = new CrossPlatformTCPSocket();
		server->initSocket(8887);
		server->bindSocket();
		server->listenForRequest(0);
		while(1){
			server->acceptConnection(0);
			server->sendToSocket((char*)"Hello world!");
			server->closeCurrentConnection(0);
		}
		delete server;
	}

}