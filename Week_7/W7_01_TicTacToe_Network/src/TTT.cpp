
#include "TTT.h"

TTTGame::TTTGame( int board_size, int in_row_to_win ):
IN_ROW_TO_WIN( in_row_to_win ), BOARD_SIZE( board_size )
{
    g_game_area = new int*[ BOARD_SIZE ];
    for( int i = 0; i < BOARD_SIZE; ++i ){
        g_game_area[i] = new int [ BOARD_SIZE ];
    }
}

void TTTGame::initBoard(){

	memset( g_game_area , 0 , BOARD_SIZE * BOARD_SIZE * sizeof(int) );

}


bool TTTGame::isEmpty(unsigned int row,unsigned int col){
    if(row < BOARD_SIZE && col < BOARD_SIZE && g_game_area[row][col])
		return false;
    return true;
}

bool TTTGame::makeMove(unsigned int row, unsigned int col, unsigned int player){
    if(row < BOARD_SIZE && col < BOARD_SIZE && isEmpty( row, col ) ) {
        g_game_area[ row ][ col ] = player;
        return true;
    }
    return false;
}

int TTTGame::getItem( unsigned int row, unsigned int col ){
    if( col < BOARD_SIZE && row < BOARD_SIZE )
		return g_game_area[ row ][ col ];
    return 0;
}

int TTTGame::isWon( unsigned int player ) {
    unsigned int wonInLine = 0;

    //checking rows
    //for each row
    for( unsigned int row = 0; row < BOARD_SIZE; ++row)
        //for each place that is less than should be to win
        for( unsigned int i = 0; i <= BOARD_SIZE-IN_ROW_TO_WIN; ++i ) {
            //check if all what should marked to win is marked
            for( unsigned int j = i; j < i+IN_ROW_TO_WIN; ++j ) {
                if( g_game_area[row][j] == player )
					++wonInLine;
                else 
					wonInLine = 0;
                if( wonInLine >= IN_ROW_TO_WIN )
					return player;
            }
            wonInLine = 0;
        }

    //checking cols
    //for each col
    for(unsigned int col = 0; col < BOARD_SIZE; col++)
        //for each place in col what is less than it necessary to win
        for(unsigned int i = 0; i <= BOARD_SIZE-IN_ROW_TO_WIN; i++){
            //check if all that it should be to win is checked
            for(unsigned int j = i; j < i+IN_ROW_TO_WIN; j++){
                if( g_game_area[j][col] == player )wonInLine++;
                else wonInLine=0;
                if(wonInLine >= IN_ROW_TO_WIN){
                    return player;
                }
            }
            wonInLine = 0;
        }



    //checking diagonals from top-left to bottom-right
    //for each row
    for(unsigned int row = 0; row <= BOARD_SIZE-IN_ROW_TO_WIN; row++)
        //for each col in row what is less than it necessary to win
        for(unsigned int col = 0; col <= BOARD_SIZE-IN_ROW_TO_WIN; col++){
            //check if all that it should be to win is checked
            for(unsigned int j = col; j < col+IN_ROW_TO_WIN; j++){
                if(g_game_area[j][j]==player)wonInLine++;
                else wonInLine=0;
                if(wonInLine>=IN_ROW_TO_WIN)return player;
            }
            wonInLine = 0;
        }

    //checking diagonals from bottom-left to top-right
    //for each row
    for(unsigned int row = IN_ROW_TO_WIN-1; row < BOARD_SIZE; row++)
        //for each col in row what is less than it necessary to win
        for(unsigned int col = 0; col <= BOARD_SIZE-IN_ROW_TO_WIN; col++){
            //check if all that it should be to win is checked
            for(unsigned int j = 0; j < IN_ROW_TO_WIN; j++){
                if(g_game_area[row-j][col+j]==player)wonInLine++;
                else wonInLine=0;
                if(wonInLine>=IN_ROW_TO_WIN)return player;
            }
            wonInLine = 0;
        }

    //check if there are some free places
    int notMarked = BOARD_SIZE * BOARD_SIZE;
    for(unsigned int row = 0; row < BOARD_SIZE; row++)
        for(unsigned int col = 0; col < BOARD_SIZE; col++)
            if(g_game_area[row][col])notMarked--;
    //if there are nothing left return -1
    if(!notMarked)return -1;

    //game goes but noone still win
    return 0;
}