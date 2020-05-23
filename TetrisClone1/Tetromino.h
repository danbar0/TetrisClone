#pragma once
#include "ITetromino.h"
#include <array>

#define MAX_SHAPE_SIZE 16

class Tetromino : public ITetromino
{
public:
	using Shape = std::array<unsigned short, MAX_SHAPE_SIZE>;

	Tetromino(Shape shape);
	Tetromino() = delete;

	~Tetromino(); 

	void Rotate() override; 

private:
	Shape shape;
};

