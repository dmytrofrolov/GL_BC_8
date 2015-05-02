//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Cross-platform Test Server.
//

#include <stdlib.h>	// atoi()
#include <stdio.h>	// printf()
#include <string.h>


#ifdef _WIN32

#include <io.h>
#include <winsock2.h>

#endif // _WIN32


#ifdef __linux__

#include <unistd.h>		// for close()
#include <netinet/in.h> // sockaddr_in structure
#include <sys/socket.h> // accept(), bind(), connect()...

#define LENGTH 512

#endif // __linux__


int main ( int argc, char *argv[] ) {

	if ( argc != 2 ) {
		printf("ERROR! Using : server <port>\n");
		exit(1);
	}

	int port = atoi(argv[1]);


	#ifdef WIN32
		
		WSADATA wsa;
		
		SOCKET listen_socket , reply_socket;
		
		struct sockaddr_in addr;
		
		int sockaddr_in_size = sizeof(struct sockaddr_in);
		
		char *message;
		
		printf("\nInitialising Winsock...");

		if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 ) {
			printf( "Failed. Error Code : %d", WSAGetLastError() );
			return 1;
		}
		printf( "Initialised.\n" );

		//Create a socket
		listen_socket = socket( AF_INET , SOCK_STREAM , 0 );
		
		if ( listen_socket == INVALID_SOCKET ) {
			printf( "Could not create socket : %d" , WSAGetLastError() );
		}

		printf( "Socket created.\n" );

		//Prepare the sockaddr_in structure
		addr.sin_family 	 = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port 		 = htons( port );

		//Bind
		if ( bind(listen_socket , (struct sockaddr *)&addr , sizeof(addr) ) == SOCKET_ERROR ) {
			printf( "Bind failed with error code : %d" , WSAGetLastError() );
		}

		printf( "Bind done\n" );
		
		//Listen to incoming connections
		listen( listen_socket , 3 );

		while ( 1 ) {
			//Accept and incoming connection
			printf("Waiting for incoming connections...\n");

			reply_socket = accept(listen_socket , (struct sockaddr *)&addr, &sockaddr_in_size);
			
			if (reply_socket == INVALID_SOCKET)
			{
				printf("accept failed with error code : %d" , WSAGetLastError());
			}

			printf("Connection accepted\n");
			
			//Reply to client
			message = (char*)"Hello World!\n";

			send( reply_socket , message , strlen(message) , 0 );

			closesocket( reply_socket );

		}

		closesocket( listen_socket );
		WSACleanup();

		system("pause");
		return 0;

	#endif // _WIN32

	#ifdef __linux__

		int reply_socket, listen_socket;
	    struct sockaddr_in addr;
	    char buf[LENGTH];
	    int bytes_read;

	    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	    if(listen_socket < 0)
	    {
	        perror("socket");
	        exit(1);
	    }
	    
	    addr.sin_family 	 = AF_INET;
	    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    addr.sin_port 		 = htons(port);

	    if(bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	    {
	        perror("bind");
	        exit(2);
	    }

	    listen(listen_socket, 1);
	    
	    while(1)
	    {
	    	printf("Wait for connection...\n");
	        
	        reply_socket = accept(listen_socket, NULL, NULL);

	        if(reply_socket < 0)
	        {
	            perror("accept");
	            exit(3);
	        }

	        bytes_read = recv(reply_socket, buf, LENGTH, 0);
	        if ( bytes_read <= 0 ) {
	        	break;
	        }

	        printf( "BYTES_READ : %d\n", bytes_read );
	        
	        send( reply_socket, buf, bytes_read, 0 );
	        
	        buf[ bytes_read ] = '\0';
	        
	        printf("%s\n", buf);

	        close( reply_socket );
	    
	        
	    }

	    close( listen_socket );

	#endif  // __linux__

	return 0;
}