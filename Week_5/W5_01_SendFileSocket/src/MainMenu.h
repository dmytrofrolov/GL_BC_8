//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Main Menu class.
// 

#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

class MainMenu{

enum MenuChoices{
	SERVER = '1',
	CLIENT,
	OPTIONS,
	EXIT
};

enum OptionsChoices{
	CHANGE_HOST = '1',
	CHANGE_PORT,
	BACK_TO_MAIN
};

enum Errors{
	CLIENT_ERROR = 1,
	SERVER_ERROR
};

private:
	char * host_;
	unsigned int port_;

	// Print menu to console
	// @return void
	void printMain( void );

	// Print options to console and make changes ( port or host )
	// @return void
	void showOptions( void );

	// Flush all symbols from stdin
	// @return void
	void flushInput( void );

	// get new host from stdin
	// @return void
	void changeHost( void );

	// get new port from stdin
	// @return void
	void changePort( void );

	//  Start server
	//  @return 0 if success or error code
	int startServer( void );

	// Start client
	// @return 0 if success or error code
	int startClient( void );

	// Clear console window
	// @return void
	static void clearScreen( void );

public:

	//  Constructor for fields inits
	MainMenu( char * host, unsigned int port );

	//  show menu to the screen and choose client or server
	//  return void
	void startMenu( void );

	// Destructor frees memory
	~MainMenu();

};


#endif // _MAIN_MENU_H_


/// EOF MainMenu.h