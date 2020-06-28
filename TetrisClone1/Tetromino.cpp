#include "pch.h"
#include "Tetromino.h"

Tetromino::Tetromino(Shape shape) :
	shape(shape)
{

}


Tetromino::~Tetromino()
{

}

void Tetromino::Rotate() {
	Tetromino::Shape rotatedShape{ 0 };
	int index;

	for (int i = 0; i < shape.size(); i++) {
		index = sideLength - 1 - yIndexAddition(i) + ((i % sideLength) * sideLength);
		rotatedShape[i] = shape[index];
	}

	shape = rotatedShape; 
}

uint8_t Tetromino::yIndexAddition(uint8_t index) {
	if (index >= 0 && index < sideLength) return 0;
	if (index >= sideLength && index < sideLength * 2) return 1;
	if (index >= sideLength * 2 && index < sideLength * 3) return 2;
	if (index >= sideLength * 3 && index < sideLength * 4) return 3;

	return 0;
}