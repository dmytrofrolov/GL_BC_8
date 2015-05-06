/**
*
*
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

typedef int (*FunctionFunc)();

#define KEY_UP 119 //72
#define KEY_DOWN 115 //80
#define KEY_LEFT 97 // 75
#define KEY_RIGHT 100 //77
#define KEY_SPACE 32

/////////////////////////////////////////////////////////////////////////////////

TTT_Gui::TTT_Gui(){
	hInstLibrary = LoadLibrary(_T("TicTacToe_Engine.dll"));

	if (hInstLibrary){

		initBoard = (t_initBoard)GetProcAddress(hInstLibrary, "initBoard");
		isEmpty = (t_isEmpty)GetProcAddress(hInstLibrary, "isEmpty");
		makeMove = (t_makeMove)GetProcAddress(hInstLibrary, "makeMove");
		getItem = (t_getItem)GetProcAddress(hInstLibrary, "getItem");
		isWon = (t_isWon)GetProcAddress(hInstLibrary, "isWon");
		
	}else{
		std::cout << "Dll not working! You are a deer =(" << std::endl;
	}

	cursor_pos_ = -1;
}

int TTT_Gui::startMenu(){
	initBoard();
	cout << "This is menu" << endl;

    //current player
    int current_player = 2;

    //user won (0 - game goes, 1 - 1st won, 2-2nd won, -1 - noone wons)
    int currentWon = 0;

    //for input choice
    int x = 1, y = 1;
	int row = 1, col = 1;
	system("cls");
	printBoard();
	drawCursor(row,col);
	char c = 0;
	while(c!=27 && !currentWon && currentWon!=-1){
		c = getch();
		switch(c){
		case KEY_UP:{
						--row;
						if(row<0)row=2;
					}
					break;

		case KEY_DOWN:{
						++row;
						if(row>2)row=0;
					}
					break;

		case KEY_LEFT:{
			--col;
			if(col<0)col=2;
		}
		break;

		case KEY_RIGHT:{
			++col;
			if(col>2)col=0;
		}
		break;

		case KEY_SPACE:{
			if(isEmpty(row,col)){
				drawPlayer(row, col, current_player);
				makeMove(row, col, current_player);
				currentWon = isWon(current_player);
				current_player = current_player==1?2:1;
			}
		}
		break;

		}
		drawCursor(row,col);
	}
		
	return 0;
}

void TTT_Gui::printBoard(){

	//char playersCh[] = { '-', 'X', '0'};

	{ // top line
		cout << (unsigned char)201;
		for(int i =0; i<19;++i)cout << (unsigned char)205;
		cout << (unsigned char)203;
		for(int i =0; i<19;++i)cout << (unsigned char)205;
		cout << (unsigned char)203;
		for(int i =0; i<19;++i)cout << (unsigned char)205;
		cout << (unsigned char)187;
	}
	cout << endl;
	for(int j = 0; j<2; j++)
	{
		{	// filling
			for(int i = 0; i<11; ++i){
				for(int j =0; j<3;++j){
					cout << (unsigned char)186;
					for(int i =0; i<19;++i)cout << " " ;
				}
				cout << (unsigned char)186;	
				cout << endl;
			}

		}
		{	// middle line
			cout << (unsigned char)204;
			for(int i =0; i<19;++i)cout << (unsigned char)205;
			cout << (unsigned char)206;
			for(int i =0; i<19;++i)cout << (unsigned char)205;
			cout << (unsigned char)206;
			for(int i =0; i<19;++i)cout << (unsigned char)205;
			cout << (unsigned char)185;
		}
		cout << endl;
	}
	{	// filling
		for(int i = 0; i<11; ++i){
			for(int j =0; j<3;++j){
				cout << (unsigned char)186;
				for(int i =0; i<19;++i)cout << " " ;
			}
			cout << (unsigned char)186;	
			cout << endl;
		}

	}
	{	// bottom line
		cout << (unsigned char)200;
		for(int i =0; i<19;++i)cout << (unsigned char)205;
		cout << (unsigned char)202;
		for(int i =0; i<19;++i)cout << (unsigned char)205;
		cout << (unsigned char)202;
		for(int i =0; i<19;++i)cout << (unsigned char)205;
		cout << (unsigned char)188;
	}
	cout << endl;
}

void gotoxy(int x, int y) 
{ 
    COORD coord;
    coord.X = x; 
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void TTT_Gui::drawCursor(unsigned int row,unsigned int col){
	if( row > 2 || col > 2 )return;
	if(cursor_pos_ < 0){
		cursor_pos_ = row * 3 + col;}
	else{
		gotoxy((cursor_pos_%3) * 20 + 2, (cursor_pos_/3) * 12 + 11 );
		for(int i = 0; i < 17;++i)cout << (unsigned char)' ';
	}
	gotoxy(col * 20 + 2, row * 12 + 11);
	for(int i = 0; i < 17;++i)cout << (unsigned char)177;

	cursor_pos_ = row * 3 + col;
}

void TTT_Gui::drawPlayer(unsigned int row,unsigned int col, int player){
	char playersCh[] = { '-', 'X', '0'};
	gotoxy(col * 20 + 2, row * 12 + 2);
	cout << playersCh[player];
}

TTT_Gui::~TTT_Gui(){
	FreeLibrary(hInstLibrary);
}