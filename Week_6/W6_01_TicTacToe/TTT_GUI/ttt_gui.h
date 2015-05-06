#ifndef _TTT_GUI_H
#define _TTT_GUI_H

#include <Windows.h>
#include <iomanip>

typedef void  (*t_initBoard)();
typedef bool  (*t_isEmpty)( unsigned int, unsigned int );
typedef bool  (*t_makeMove)(unsigned int row, unsigned int col, unsigned int player);
typedef unsigned int  (*t_getItem)(unsigned int row, unsigned int col);
typedef int (*t_isWon)(unsigned int player);


class TTT_Gui{
private:
	HINSTANCE hInstLibrary;
	int cursor_pos_;
public:
	t_initBoard initBoard;
	t_isEmpty isEmpty;
	t_makeMove makeMove;
	t_getItem getItem;
	t_isWon isWon;

	TTT_Gui();
	int initDLL();
	int startMenu();
	void printBoard();
	void drawCursor(unsigned int row, unsigned int col);
	void drawPlayer(unsigned int row,unsigned int col, int player);

	~TTT_Gui();

};



#endif // _TTT_GUI_H