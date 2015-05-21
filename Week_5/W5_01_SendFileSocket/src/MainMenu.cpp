//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Main Menu class.
// 

#include "MainMenu.h"
#include "CrossPlatformTCPSocket.h"
#include "Server.h"
#include "Client.h"
#include <cstdio>

#define SCANF_ERROR 2

/////////////////////////////////////////////////////////////////////////////

MainMenu::MainMenu( char * host,  unsigned int port ):
	port_ ( port )
{
	host_ = new char [ strlen( host ) + 1 ];	// +1 for '\0'
	strcpy( host_, host );
}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::startMenu(){
	char menu_input = 0;
	int scanf_result = 0;

	// repeat until user choose exit
	while( menu_input != EXIT ){
		printMain();

		// get user input
		scanf_result = scanf("\n%c", &menu_input );

		if( scanf_result != SCANF_ERROR ){
			switch( menu_input ){
				case SERVER:
					startServer();
					break;
				case CLIENT:
					startClient();
					break;
				case OPTIONS:
					showOptions();
					break;
			}
		}

	}

}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::printMain(){
	clearScreen();
	printf("HOST : %s\tPORT : %d\n", host_, port_ );
	printf("Menu:\n1 - Start Server\n2 - Start Client\n");
	printf("3 - Options\n4 - Exit\n");
}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::showOptions(){

	char option_input = 0;
	int scanf_result = 0;

	// repeat until user choose go back to main
	while( option_input != BACK_TO_MAIN ) {
		clearScreen();
		printf("HOST : %s\tPORT : %u\n", host_, port_ );
		printf("Options\n1 - Change host\n2 - Change port\n3 - Back to main...\n");
		
		scanf_result = scanf("\n%c", &option_input );
		
		if( scanf_result != SCANF_ERROR ) {

			switch( option_input ){

				case CHANGE_HOST:
					changeHost();
					break;

				case CHANGE_PORT:
					changePort();
					break;
			}
			
			flushInput();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::clearScreen(){
	#ifdef _WIN32_
		system("cls");
	#endif
	#ifdef __linux__
		system("clear");
	#endif
}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::flushInput(){
	int c;
	while( ( c = getchar() ) != '\n' && c != EOF );
}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::changeHost(){
	printf("Enter new HOST ( max 30 symbols ) : \n");
	
	char host[ 30 ];
	
	int scanf_result;
	
	scanf_result = scanf("\n%29s", host);
	
	if( scanf_result != SCANF_ERROR ){
		delete [] host_;
		host_ = new char [ strlen( host ) + 1 ];
		strcpy( host_, host );
	}
}

/////////////////////////////////////////////////////////////////////////////

void MainMenu::changePort(){
	printf("Enter new PORT : \n");
	
	unsigned int port;
	int scanf_result;
	
	scanf_result = scanf("\n%6u", &port);
	
	if( scanf_result != SCANF_ERROR ){
		port_ = port;
	}
}

/////////////////////////////////////////////////////////////////////////////

int MainMenu::startServer(){
	Server * server = new Server();

	int init_result = server->initServer( port_, 10 );
	if( init_result != SUCCESS_RESULT ){
		printf("Server init error, code : %d\n", init_result );
	}
	int start_result = server->startServer();

	if( start_result != SUCCESS_RESULT ){
		printf("Server start error, code : %d\n", start_result );
	}

	int stop_result = server->stopServer();
	
	if( stop_result != SUCCESS_RESULT ){
		printf("Server stop error, code : %d\n", stop_result );
	}

	delete server;

	printf("Press Enter to continue...\n");
	flushInput();
	getchar();

	// if there was some errors let user to see it and Press key
	if( init_result != SUCCESS_RESULT || start_result != SUCCESS_RESULT || stop_result != SUCCESS_RESULT ){
		return SERVER_ERROR;
	}

	return SUCCESS_RESULT;
}

/////////////////////////////////////////////////////////////////////////////

int MainMenu::startClient(){
	
	char file_name[ FILE_NAME_SIZE ];
	
	printf("File name : \n");
	
	scanf("%29s", file_name );
	
	Client * client = new Client();
	
	int init_result = client->initClient( host_, port_ );

	if( init_result != SUCCESS_RESULT ){
		printf("Client init error, code : %d\n", init_result );
	}

	int get_result = client->getFile( file_name );

	if( get_result != SUCCESS_RESULT ){
		printf("Client get file error, code : %d\n", get_result );
	}

	delete client;

	printf("Press any key to continue...\n");
	flushInput();
	getchar();

	// if there was some errors let user to see it and Press key
	if( init_result != SUCCESS_RESULT  || get_result != SUCCESS_RESULT ){
		return CLIENT_ERROR;
	}

	return SUCCESS_RESULT;

}

/////////////////////////////////////////////////////////////////////////////

MainMenu::~MainMenu(){
	delete [] host_;
	host_ = NULL;

}

/// EOF MainMenu.cpp