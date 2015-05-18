//
// Copyright 2015. Dmytro Frolov.  All Rights Reserved.
// 
// Main Menu class.
// 

#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

class MainMenu{
private:
	char * host_;
	char * port_;
public:

	//  Constructor for fields inits
	MainMenu();

	//  show menu to the screen and choose client or server
	void startMenu();

	//  start server
	void startServer();

	// start port
	void startClient();
};


#endif // _MAIN_MENU_H_


/// EOF MainMenu.h