// TicTacToe_Engine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

int g_game_area[3][3];
const int BOARD_SIZE = 3;
const int IN_ROW_TO_WIN = 3;

extern "C" {

#define DLLEXP __declspec(dllexport)

int DLLEXP f();
void DLLEXP initBoard();
bool DLLEXP isEmpty( unsigned int, unsigned int );
bool DLLEXP makeMove(unsigned int row, unsigned int col, unsigned int player);
unsigned int DLLEXP getItem(unsigned int row, unsigned int col);
int DLLEXP isWon(unsigned int player);
int DLLEXP aiMove(int ai_player);


void initBoard(){

	memset( g_game_area, 0 , 9 );
	for(int i = 0; i<3; ++i)
		for(int j =0; j < 3; ++j)
			g_game_area[i][j]=0;
}


bool isEmpty(unsigned int row,unsigned int col){
    if(row < BOARD_SIZE && col < BOARD_SIZE && g_game_area[row][col])return false;
    return true;
}

bool makeMove(unsigned int row, unsigned int col, unsigned int player){
    if(row < BOARD_SIZE && col < BOARD_SIZE && isEmpty(row,col)){
        g_game_area[row][col]=player;
        return true;
    }
    return false;
}

unsigned int getItem(unsigned int row, unsigned int col){
    if(col<BOARD_SIZE && row < BOARD_SIZE)return g_game_area[row][col];
    return 0;
}

int isWon(unsigned int player){
    unsigned int wonInLine = 0;

    //checking rows
    //for each row
    for(unsigned int row = 0; row < BOARD_SIZE; row++)
        //for each place that is less than should be to win
        for(unsigned int i = 0; i <= BOARD_SIZE-IN_ROW_TO_WIN; i++){
            //check if all what should marked to win is marked
            for(unsigned int j = i; j < i+IN_ROW_TO_WIN; j++){
                if(g_game_area[row][j]==player)wonInLine++;
                else wonInLine=0;
                if(wonInLine>=IN_ROW_TO_WIN)return player;
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
                if(g_game_area[j][col]==player)wonInLine++;
                else wonInLine=0;
                if(wonInLine>=IN_ROW_TO_WIN){return player;}
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


int aiMove( int ai_player ){
	int col, row;
	int * game_area = &g_game_area[0][0];
	//g_game_area
	col = 1;
	row = 1;
	int sum;
	
	// is winnable rows
	sum = 0;
	for(row = 0; row < 3; ++row){
		for(col = 0; col < 3; ++col){
			sum+= *(game_area + row * 3 + col);
		}
		if(sum == 10){
			for(col = 0; col<3; ++col){
				if(isEmpty(row,col)){
					makeMove(row, col, ai_player);
					return row * 3 + col;
				}
			}
		}
		sum = 0;
	}

	// is winnable cols
	for(col = 0; col < 3; ++col){
		for(row = 0; row < 3; ++row){
			sum+= *(game_area + row * 3 + col);
		}
		if(sum == 10){
			for(row = 0; row<3; ++row){
				if(isEmpty(row,col)){
					makeMove(row, col, ai_player);
					return row * 3 + col;
				}
			}
		}
		sum = 0;
	}

	// is winnable diagonals
	for(row = 0; row < 3; ++row){
		sum+= *(game_area + row * 3 + row);
	}
	if(sum == 10){
		for(row = 0; row<3; ++row){
			if(isEmpty(row,row)){
				makeMove(row, row, ai_player);
				return row * 3 + row;
			}
		}
	}
	sum = 0;
	for(row = 0; row < 3; ++row){
		sum+= *(game_area + row * 3 + 2-row);
	}
	if(sum == 10){
		for(row = 0; row<3; ++row){
			if(isEmpty(row,2-row)){
				makeMove(row, 2-row, ai_player);
				return row * 3 + 2-row;
			}
		}
	}
	sum = 0;

	/// if user winnable
	
	// is winnable rows
	sum = 0;
	for(row = 0; row < 3; ++row){
		for(col = 0; col < 3; ++col){
			sum+= *(game_area + row * 3 + col);
		}
		if(sum == 2){
			for(col = 0; col<3; ++col){
				if(isEmpty(row,col)){
					makeMove(row, col, ai_player);
					return row * 3 + col;
				}
			}
		}
		sum = 0;
	}

	// is winnable cols
	for(col = 0; col < 3; ++col){
		for(row = 0; row < 3; ++row){
			sum+= *(game_area + row * 3 + col);
		}
		if(sum == 2){
			for(row = 0; row<3; ++row){
				if(isEmpty(row,col)){
					makeMove(row, col, ai_player);
					return row * 3 + col;
				}
			}
		}
		sum = 0;
	}

	// is winnable diagonals
	for(row = 0; row < 3; ++row){
		sum+= *(game_area + row * 3 + row);
	}
	if(sum == 2){
		for(row = 0; row<3; ++row){
			if(isEmpty(row,row)){
				makeMove(row, row, ai_player);
				return row * 3 + row;
			}
		}
	}
	sum = 0;

	for(row = 0; row < 3; ++row){
		sum+= *(game_area + row * 3 + 2-row);
	}
	if(sum == 2){
		for(row = 0; row<3; ++row){
			if(isEmpty(row,2-row)){
				makeMove(row, 2-row, ai_player);
				return row * 3 + 2-row;
			}
		}
	}
	sum = 0;

	// if center free
	if( isEmpty(1,1) ){
		makeMove(1, 1, ai_player);

		return 1 * 3 + 1;
	}


	// if not winnable or not center
	for(int i = 0; i < 9; ++i){
		if(isEmpty(i/3,i%3)){
			makeMove(i/3, i%3, ai_player);
			return i;
		}
	}



	return 0;
}


}