// TetrisClone1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <map>
#include "Tetromino.h"
#include "ConsolePlayingField.h"
#include "KeyboardInput.h"
#include "Tetris.h"

using namespace std; 

void customDelayFunc(void) {
	this_thread::sleep_for(100ms); // Small Step = 1 Game Tick
}

int main()
{
	ConsolePlayingField output(STANDARD_WIDTH, STANDARD_HEIGHT);
	KeyboardInput input;
	Tetris tetris(input, output, customDelayFunc);

	tetris.Run(); 

	return 0; 
}
