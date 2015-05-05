// TicTacToe_Engine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

int g_game_area[3][3];

extern "C" {

#define DLLEXP __declspec(dllexport)

int DLLEXP f();

int f(){
	return 15;
}



}