//

#include <cstring>

#ifndef _TTT_H_
#define _TTT_H_

class TTTGame{
private:
	int **g_game_area;
	const int BOARD_SIZE;
	const int IN_ROW_TO_WIN;
	
public:
	
	TTTGame( int board_size, int in_row_to_win )
	
	bool isEmpty( unsigned int row, unsigned int col );
	
	bool makeMove( unsigned int row, unsigned int col, unsigned int player );
	
	int getItem( unsigned int row, unsigned int col );
	
	int isWon( unsigned int player );
	
	int aiMove( int ai_player );
	
	~TTTGame();
};


#endif // _TTT_H_

// EOF TTT.h