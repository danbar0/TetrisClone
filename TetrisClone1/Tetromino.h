#pragma once
#include "ITetromino.h"
#include <array>

class Tetromino : public ITetromino
{
public:
	static constexpr uint8_t sideLength = 4;
	using Shape = std::array<bool, sideLength * sideLength>;

	Tetromino(Shape shape, char displayCharacter);
	Tetromino() = delete;

	~Tetromino(); 

	void Rotate() override; 

private:
	uint8_t yIndexAddition(uint8_t index);
	Shape shape;
	char displayCharacter;
};

