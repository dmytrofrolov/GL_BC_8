// TicTacToe gui lib geader file
// @date May 2015
// @author Dmytro Frolov

#ifndef _TTT_GUI_H
#define _TTT_GUI_H

#include <Windows.h>
#include <iomanip>
#include "TTT.h"


class TTT_Gui{
private:

	TTTGame * game_;

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