// TetrisClone1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <map>
#include "ConsolePlayingField.h"
#include "KeyboardInput.h"
#include "Tetris.h"

using namespace std; 

/*-----------------------------------------------------------------------------------------------*/
void customDelayFunc(void) {
	this_thread::sleep_for(16ms); // Rougly 60 fps
}

/*-----------------------------------------------------------------------------------------------*/
int main()
{
	ConsolePlayingField outputWindow(STANDARD_WIDTH, STANDARD_HEIGHT);
	KeyboardInput input;
	Tetris tetris(input, outputWindow, customDelayFunc);

	tetris.Run(); 

	return 0; 
}
