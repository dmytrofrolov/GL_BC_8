#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/// ____ WINDOWS ____
#ifdef _WIN32

#include <io.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#endif

/// ____ LINUX ____
#ifdef __linux__

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

int main ()
{

#ifdef WIN32
	
	WSADATA wsa;
	SOCKET listenSocket , replySocket;
	struct sockaddr_in addr;
	int sockaddr_in_size = sizeof(struct sockaddr_in);
	char *message;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");

	//Create a socket
	if ((listenSocket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( 8888 );

	//Bind
	if ( bind(listenSocket , (struct sockaddr *)&addr , sizeof(addr)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d" , WSAGetLastError());
	}

	puts("Bind done");
	
	//Listen to incoming connections
	listen(listenSocket , 3);

	while (1)
	{


		//Accept and incoming connection
		puts("Waiting for incoming connections...");

		replySocket = accept(listenSocket , (struct sockaddr *)&addr, &sockaddr_in_size);
		
		if (replySocket == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d" , WSAGetLastError());
		}

		puts("Connection accepted");
		//Reply to client
		message = "Hello Client , I have received your connection. But I have to go now, bye\n";
		send(replySocket , message , strlen(message) , 0);

		closesocket(replySocket);

	}

	closesocket(listenSocket);
	WSACleanup();
	system("pause");
	return 0;

#endif

#ifdef __linux__

	int replySocket, listener;
    struct sockaddr_in addr;
    char buf[LENGTH];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(3425);

    if(bind(listenSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listenSocket, 1);
    
    while(1)
    {
    	printf("Wait for connection...\n");
        replySocket = accept(listenSocket, NULL, NULL);
        if(replySocket < 0)
        {
            perror("accept");
            exit(3);
        }

        bytes_read = recv(replySocket, buf, LENGTH, 0);
        if(bytes_read <= 0) break;
        printf("BYTES_READ : %d\n", bytes_read );
        send(replySocket, buf, bytes_read, 0);
        printf("%s\n", buf);

        close(replySocket);
    
        
    }

    close(listenSocket);

#endif

return 0;
}