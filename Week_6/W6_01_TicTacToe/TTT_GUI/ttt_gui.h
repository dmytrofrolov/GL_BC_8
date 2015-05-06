#ifndef _TTT_GUI_H
#define _TTT_GUI_H

#include <Windows.h>
#include <iomanip>

typedef void  (*t_initBoard)();
typedef bool  (*t_isEmpty)( unsigned int, unsigned int );
typedef bool  (*t_makeMove)(unsigned int row, unsigned int col, unsigned int player);
typedef unsigned int  (*t_getItem)(unsigned int row, unsigned int col);
typedef int (*t_isWon)(unsigned int player);
typedef int (*t_aiMove)(int ai_player);


class TTT_Gui{
private:
	t_initBoard initBoard;
	t_isEmpty isEmpty;
	t_makeMove makeMove;
	t_getItem getItem;
	t_isWon isWon;
	t_aiMove aiMove;

	HINSTANCE hInstLibrary;
	int cursor_pos_;

	int initDLL();
	void printBoard();
	void drawCursor(unsigned int row, unsigned int col);
	void drawPlayer(unsigned int row,unsigned int col, int player);
	int startGame(int mode);

public:
	TTT_Gui();
	int startMenu();
	~TTT_Gui();

};



#endif // _TTT_GUI_H