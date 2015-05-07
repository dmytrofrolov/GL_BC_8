/**
*  TicTacToe, GUI Lib
*
*	@date May, 2015
*	@author  Dmytro Frolov
*
*/
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <tchar.h>

#include "ttt_gui.h"

using std::cout;
using std::cin;
using std::endl;

enum keys{
		KEY_UP		= 119, // W
		KEY_DOWN	= 115, // S
		KEY_LEFT	= 97, // A
		KEY_RIGHT	= 100, // D
		KEY_SPACE	= 32,
		KEY_ESC		= 27
	};

enum gameModes{
	SINGLE_PLAYER = 0,
	MULTIPLAYER = 1
};

enum players{
	USER_X = 1,
	USER_0 = 5
};

void gotoxy(int x, int y) 
{ 
    COORD coord;
    coord.X = x; 
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/////////////////////////////////////////////////////////////////////////////////

TTT_Gui::TTT_Gui(){

	hInstLibrary_ = LoadLibrary( _T( "TicTacToe_Engine.dll" ) );

	if ( hInstLibrary_ ){

		initBoard = ( t_initBoard )GetProcAddress( hInstLibrary_, "initBoard" );
		isEmpty   = ( t_isEmpty )  GetProcAddress( hInstLibrary_, "isEmpty" );
		makeMove  = ( t_makeMove ) GetProcAddress( hInstLibrary_, "makeMove" );
		getItem   = ( t_getItem )  GetProcAddress( hInstLibrary_, "getItem" );
		isWon     = ( t_isWon )	   GetProcAddress( hInstLibrary_, "isWon" );
		aiMove    = ( t_aiMove )   GetProcAddress( hInstLibrary_, "aiMove" );
		
	}else{
		std::cout << "Dll not working! You are a deer =(" << std::endl;
	}

	// mark cursor as never used
	cursor_pos_ = -1;
}

/////////////////////////////////////////////////////////////////////////////////

int TTT_Gui::startMenu(){
	// if dll is not loaded
	if(!hInstLibrary_)
		return 2;

	int mode = SINGLE_PLAYER; // 0 - single player, 1 - multiplayer

	char logo[] = {
		"#######          #######               #######               "
		"   #    #  ####     #      ##    ####     #     ####  ###### "
		"   #    # #    #    #     #  #  #    #    #    #    # #      "
		"   #    # #         #    #    # #         #    #    # #####  "
		"   #    # #         #    ###### #         #    #    # #      "
		"   #    # #    #    #    #    # #    #    #    #    # #      "
		"   #    #  ####     #    #    #  ####     #     ####  ###### "
	};

	for(int i = 0; i < 7; ++i){
		for(int j = 0; j < 61; ++j){
			cout << logo[i * 61 + j];
		}
		cout << endl;
	}

	cout << endl;

	cout << " What mode do you want to choose : \n Single player - press 1\n Multiplayer - press 2 \n (or ESC to exit) \n:";

	// char for input
	char input_ch = 0;
	input_ch = _getch();
	if( input_ch == KEY_ESC )
		return 0;

	if( input_ch == '2' )
		mode = MULTIPLAYER; // mark for multiplayer
	
	system("cls");

	cout << "To play use W A S D for moves and Space for choice!\n";
	cout << "\nPress Any key to start...";

	_getch();
	
	while(1)
	{
		startGame( mode );
		cout << "Press any key to continue... or ESC to exit\n";
		input_ch = _getch();

		if( input_ch == KEY_ESC ){
			break;
		}
	}
	cout << endl;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////

int TTT_Gui::startGame(int mode){
	char input_ch = 0;	
	int current_player;
	int current_won;
	int row = 1, col = 1;

	initBoard();

	current_player = 1;	// 1 - USER, 5 - will be AI

	//user won (0 - game goes, 1 - 1st won, 2-2nd won, -1 - noone wons)
	current_won = 0;

	//for input choice
	row = 1, col = 1;

	system( "cls" );
	
	printBoard();
	drawCursor( row, col );

	while( input_ch != KEY_ESC && !current_won && current_won != -1 ){
		input_ch = _getch();
		switch( input_ch ){

		case KEY_UP:{
						--row;
						if( row < 0 ){
							row=2;
						}
					}
					break;

		case KEY_DOWN:{
						++row;
						if( row > 2 ){
							row=0;
						}
					}
					break;

		case KEY_LEFT:{
			--col;
			if(col<0)col=2;
		}
		break;

		case KEY_RIGHT:{
			++col;
			if( col > 2 ){
				col = 0;
			}
		}
		break;

		case KEY_SPACE:{
			if( isEmpty( row, col ) ){
				// user
				drawPlayer( row, col, current_player );
				makeMove( row, col, current_player );
				current_won = isWon( current_player );
				current_player = current_player == USER_0 ? USER_X : USER_0;

				if( mode == SINGLE_PLAYER )
				if( !current_won && current_won != -1 ){
					// ai
					int ai_m = aiMove( current_player );
					drawPlayer( ai_m/3, ai_m%3, current_player );
					current_won = isWon( current_player );
					current_player = current_player == USER_0 ? USER_X : USER_0;
				}


			}
		}
		break;

		}
		drawCursor( row, col );
	}
	
	gotoxy( 0 , 12 * 3 + 2 );
	cout << " Game is over!\n";
	if( current_won!=-1 ){
		if( current_player == USER_0 ){
			cout << " X - won!!\n";
		}else{
			cout << " 0 - won!!\n";
		}
	}

	return 0;

}

/////////////////////////////////////////////////////////////////////////////////

void TTT_Gui::printBoard(){

	// counter
	int i,j,col,row;

	// top line
	cout << (unsigned char)201;

	for( col = 0; col < 19; ++col )
		cout << (unsigned char)205;

	cout << (unsigned char)203;
	
	for( col = 0; col < 19; ++col )
		cout << (unsigned char)205;
	
	cout << (unsigned char)203;
	
	for( col = 0; col < 19; ++col )
		cout << (unsigned char)205;

	cout << (unsigned char)187;

	cout << endl;

	// for 2 rows
	for( row = 0; row < 2; ++row )
	{
		// for each line in row, filling with empty spaces
		for( i = 0; i < 11; ++i ){

			// for each col in row
			for( int j = 0; j < 3; ++j ) {
				cout << (unsigned char)186;
				for(col = 0; col < 19; ++col )
					cout << " " ;
			}
			cout << (unsigned char)186;	
			cout << endl;
		}

		
		// middle line
		cout << (unsigned char)204;
		
		for( col = 0; col < 19; ++col )
			cout << (unsigned char)205;

		cout << (unsigned char)206;

		for( col = 0; col < 19; ++col )
			cout << (unsigned char)205;

		cout << (unsigned char)206;

		for( col = 0; col < 19; ++col )
			cout << (unsigned char)205;

		cout << (unsigned char)185;
		
		cout << endl;
	}
	
	// last, 3rd row
	// filling with empty spaces
	for( int i = 0; i < 11; ++i ){
		for( int j = 0; j < 3; ++j ){
			cout << (unsigned char)186;
			for(int i = 0; i < 19; ++i )
				cout << " " ;
		}
		cout << (unsigned char)186;	
		cout << endl;
	}

	
	// bottom line
	cout << (unsigned char)200;
	for( col = 0; col < 19; ++col )
		cout << (unsigned char)205;

	cout << (unsigned char)202;

	for( col = 0; col < 19; ++col )
		cout << (unsigned char)205;

	cout << (unsigned char)202;

	for( col = 0; col < 19; ++col )
		cout << (unsigned char)205;
	cout << (unsigned char)188;
	
	cout << endl;
}

/////////////////////////////////////////////////////////////////////////////////

void TTT_Gui::drawCursor( unsigned int row, unsigned int col ){
	if( row > 2 || col > 2 )
		return;

	if( cursor_pos_ < 0){
		cursor_pos_ = row * 3 + col;
	}
	else{
		gotoxy( (cursor_pos_%3) * 20 + 2, (cursor_pos_/3) * 12 + 11 );
		for(int i = 0; i < 17;++i)cout << (unsigned char)' ';
	}

	gotoxy( col * 20 + 2, row * 12 + 11 );
	for( int i = 0; i < 17; ++i )
		cout << (unsigned char)177;

	cursor_pos_ = row * 3 + col;
}

/////////////////////////////////////////////////////////////////////////////////

void TTT_Gui::drawPlayer( unsigned int row, unsigned int col, int player ){
	char playersCh[] = { '-', 'X',' ', ' ', ' '  ,'0'};
	char pX[] = {
		"X             X"
		"  X         X  "
		"   X       X   "
		"    X     X    "
		"      X X      "
		"     X   X     "
		"    X     X    "
		"  X         X  "
		" X           X "
		"X             X"
	};

	char p0[] = {
		"      000      "
		"    0     0    "
		"   0       0   "
		"  0         0  "
		"  0         0  "
		"  0         0  "
		"  0         0  "
		"   0       0   "
		"    0     0    "
		"      000      "
	};
	
	gotoxy( col * 20 + 2, row * 12 + 2 );

	if( player == 1 ){
		for( int i = 0; i < 10; ++i ){
			gotoxy( col * 20 + 3, row * 12 + 1 + i );
			
			for( int j = 0; j < 15; ++j ){
				cout << pX[i * 15 + j];
			}
		}

	}
	
	if( player == 5 ){
		for( int i = 0; i < 10; ++i ){
			
			gotoxy( col * 20 + 3, row * 12 + 1 + i );
			
			for( int j = 0; j < 15; ++j ){
				cout << p0[i * 15 + j];
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////////

TTT_Gui::~TTT_Gui(){
	FreeLibrary(hInstLibrary_);
}

/////////////////////////////////////////////////////////////////////////////////

/// EOL ttt_gui.cpp