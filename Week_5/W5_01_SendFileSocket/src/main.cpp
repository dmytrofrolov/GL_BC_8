//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Main file for SendFile project.
//

#include "MainMenu.h"
#include <stdio.h>
#include <stdlib.h>

#define INVALID_INPUT 2

int main( int argc, char* argv[] ){

	char * host;
	int port;
		
	if( argc == 3 ){
		host = argv[1];
		port = atoi( argv[2] );
	}
	else {
		printf("Using:\n SendFile <host> <port>\n");
		printf("Marked as default:\nHOST - localhost\nPORT - 1234\n");
		host = (char*)"localhost";
		port = 1234;
	}

	MainMenu menu( host, port );
	menu.startMenu();
	
	return 0;

}