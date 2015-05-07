// TicTacToe gui lib geader file
// @date May 2015
// @author Dmytro Frolov

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
	// pointers to functions from Dll TTT Engine
	t_initBoard initBoard;
	t_isEmpty isEmpty;
	t_makeMove makeMove;
	t_getItem getItem;
	t_isWon isWon;
	t_aiMove aiMove;

	// pointer for Dll include
	HINSTANCE hInstLibrary_;

	// position of cursor on screen (0..8)
	int cursor_pos_;


	//	@description	initialize dll with TTT Engine
	//	@param - none
	//	@return - 0 if success or error code
	int initDLL( void );


	//	@description	print board to screen
	//	@param - none
	//	@return - none
	void printBoard( void );


	//	@description	print cursor at position row, col
	//	@param	[unsigned int] - row where cursor should be
	//	@param	[unsigned int] - col where cursor should be
	//	@return - none
	void drawCursor( unsigned int row, unsigned int col );


	//	@description	draw player choice at position row col
	//	@param	[unsigned int] - row where cursor should be
	//	@param	[unsigned int] - col where cursor should be
	//	@param	[int] - player id 1 - for user_x and 5 for user_0 
	//	@return - none
	void drawPlayer( unsigned int row, unsigned int col, int player );


	//	@description	start the game session
	//	@param [int]	- mode single player or multiplayer
	//	@return - 0 if success or error code
	int startGame( int mode );

public:
	TTT_Gui();

	//	@description	start the gui game
	//	@return 0 if success or error code
	int startMenu( void );
	
	~TTT_Gui();

};



#endif // _TTT_GUI_H