// TetrisClone1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <map>
#include "Tetromino.h"
#include "ConsolePlayingField.h"
#include "KeyboardInput.h"

using namespace std; 

enum class ShapeName {
	Square,
	Line,
	RightZee,
	LeftZee,
	Tee,
	LeftBend,
	RightBend
};

Tetromino::Shape square {
	0,0,0,0,
	0,1,1,0,
	0,1,1,0,
	0,0,0,0
}; 

Tetromino::Shape line {
	0,0,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,1,0
};

Tetromino::Shape rightZee {
	0,1,0,0,
	0,1,1,0,
	0,0,1,0,
	0,0,0,0
};

Tetromino::Shape leftZee{
	0,0,1,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

Tetromino::Shape tee {
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

Tetromino::Shape leftBend {
	0,1,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,0,0
};

Tetromino::Shape rightBend{
	0,1,1,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0
};


int main()
{

    std::cout << "Hello World!\n"; 
}
