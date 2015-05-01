#include "CrossPlatformTCPSocket.h"


int CrossPlatformTCPSocket::initSocket(int port){
	addr.sin_port = htons( port );

	#ifdef _WIN32	
		printf("Initialising Winsock...\n");
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("Failed. Error Code : %d", WSAGetLastError());
			return 1;
		}
	#endif


	printf("Create Socket...\n");
	if ((io_socket = socket(AF_INET , SOCK_STREAM , 0 )) < 0)
	{
		printf("Could not create socket\n");
		#ifdef _WIN32
			printf("\n : %d\n", WSAGetLastError());
		#endif
	}

	return 0;
}

int CrossPlatformTCPSocket::bindSocket(){
	printf("Bind...\n");
	if ( bind(io_socket , (struct sockaddr *)&addr , sizeof(addr)) < 0)
	{
		printf("Bind failed with error code\n");
		#ifdef _WIN32
			printf("\n : %d\n", WSAGetLastError());
		#endif
		exit(1);
	}

	return 0;
}

int CrossPlatformTCPSocket::listenForRequest(int){
	printf("Listen for request...\n");
	listen(io_socket , 3);
	return 0;
}

int CrossPlatformTCPSocket::acceptConnection(int){
	printf("Wait to accept connection...\n");
	reply_socket = accept(io_socket , (struct sockaddr *)&addr, &sockaddr_in_size);
	if (reply_socket < 0)
	{
		printf("accept failed with error \n");
		#ifdef _WIN32
			printf("\n : %d\n", WSAGetLastError());
		#endif
		return 1;
	}
	return 0;	
}

int CrossPlatformTCPSocket::closeCurrentConnection(int){
	#ifdef _WIN32
		closesocket(reply_socket);
	#endif
	
	#ifdef __linux__
		close(reply_socket);
	#endif	
	return 0;
}

int CrossPlatformTCPSocket::connectToSocket(int size){
	//addr.sin_port = htons( port );
	inet_pton(AF_INET, "localhost", &addr.sin_addr);
	bzero(&(addr.sin_zero), 8);

	if (connect(io_socket, (struct sockaddr *)&addr, sockaddr_in_size ) < 0 )
	{
		printf("ERROR: Failed to connect to the host!\n");
		return 1;
	}
	else
		printf("[Client] Connected to server at ...ok!\n");
	return 0;
}

char * CrossPlatformTCPSocket::receiveFromSocket(int){
	size_t receiveSize = recv(io_socket, buffer, BUFFER_SIZE, 0);
	return buffer;
}

int CrossPlatformTCPSocket::sendToSocket( char * request ){
	send(reply_socket , request , strlen(request) , 0);
	return 0;
}

CrossPlatformTCPSocket::CrossPlatformTCPSocket(){
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	sockaddr_in_size = sizeof(struct sockaddr_in);
}

CrossPlatformTCPSocket::~CrossPlatformTCPSocket(){
	close (io_socket);

	#ifdef _WIN32		
		WSACleanup();
	#endif
}