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
using std::endl;

typedef int (*FunctionFunc)();

/////////////////////////////////////////////////////////////////////////////////

int TTT_Gui::startMenu(){
	cout << "This is menu" << endl;

	FunctionFunc _FunctionFunc;

	HINSTANCE hInstLibrary = LoadLibrary(_T("TicTacToe_Engine.dll"));

	if (hInstLibrary){
		_FunctionFunc = (FunctionFunc)GetProcAddress(hInstLibrary, "f");
		 if (_FunctionFunc) {
			 std::cout << _FunctionFunc();      
			 std::cout << std::endl;
		 }
		FreeLibrary(hInstLibrary);
	}else{
		std::cout << "Dll not working! You are a deer =(" << std::endl;
	}

	return 0;
}