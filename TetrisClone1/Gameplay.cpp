#include "pch.h"
#include "Gameplay.h"
#include "Tetris.h"

Gameplay::Piece test{
	1,1,1,1,
	1,1,1,1,
	1,1,1,1,
	1,1,1,1
};

Gameplay::Piece square{
	0,0,0,0,
	0,1,1,0,
	0,1,1,0,
	0,0,0,0
};

Gameplay::Piece line{
	0,0,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,1,0
};

Gameplay::Piece rightZee{
	0,1,0,0,
	0,1,1,0,
	0,0,1,0,
	0,0,0,0
};

Gameplay::Piece leftZee{
	0,0,1,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

Gameplay::Piece tee{
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0,
};

Gameplay::Piece leftBend{
	0,1,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,0,0
};

Gameplay::Piece rightBend{
	0,1,1,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0
};


Gameplay::Gameplay(IPlayerInput& input, IPlayingField& display, timeTickDelayFunc delay) :
	input(input),
	display(display),
	delay(delay)
{
	time = 1;

	pieces[PieceName::Square] = square;
	pieces[PieceName::Line] = line;
	pieces[PieceName::RightZee] = rightZee;
	pieces[PieceName::LeftZee] = leftZee;
	pieces[PieceName::Tee] = tee;
	pieces[PieceName::LeftBend] = leftBend;
	pieces[PieceName::RightBend] = rightBend;

	for (int y = 0; y < display.GetHeight(); y++) {
		for (int x = 0; x < display.GetWidth(); x++) {
			displayBuffer.push_back(' ');
		}
	}

}

Gameplay::~Gameplay()
{

}

void Gameplay::Run()
{
	uint32_t difficulty = 1000;
	auto inputs = input.GetPlayerInputs();
	auto currentPiece = pieces[PieceName::Line];
	bool rotationLock = false;
	int x = 0, y = 0;

	while (1) {
		// Timing
		delay();
		updateGameTime();

		if (time % difficulty == 0) {
			y += (doesPieceFit(currentPiece, x, y + 1)) ? 1 : 0;
		}

		// Inputs
		inputs = input.GetPlayerInputs();

		if (inputs[Command::SPACE]) {
			if (doesPieceFit(rotatePiece(currentPiece), x, y) && !rotationLock) {
				currentPiece = rotatePiece(currentPiece);
				rotationLock = true;
			}
		}
		else {
			rotationLock = false;
		}

		x += (inputs[Command::RIGHT] && doesPieceFit(currentPiece, x + 1, y)) ? 1 : 0;
		x -= (inputs[Command::LEFT] && doesPieceFit(currentPiece, x - 1, y)) ? 1 : 0;
		//y += (inputs[Command::DOWN] && doesPieceFit(currentPiece, x, y + 1)) ? 1 : 0; 

		// Game logic

		// Display 
		clearDisplayBuffer();
		drawPieceToLocation(currentPiece, x, y);

		display.UpdateDisplayBuffer(displayBuffer);
		display.Draw();
	}
}

void Gameplay::updateGameTime() {
	time++;
	if (time == 60001) time = 1;
}

void Gameplay::clearDisplayBuffer() {
	std::fill(displayBuffer.begin(), displayBuffer.end(), 0);
}

Gameplay::Piece Tetris::rotatePiece(Piece piece)
{
	Gameplay::Piece rotatedPiece{ 0 };
	int index;

	for (int i = 0; i < piece.size(); i++) {
		index = sideLength - 1 - hackyIndexGetter(i) + ((i % sideLength) * sideLength);
		rotatedPiece[i] = piece[index];
	}

	return rotatedPiece;
}

uint8_t Gameplay::hackyIndexGetter(uint8_t index) {
	if (index >= 0 && index < sideLength) return 0;
	if (index >= sideLength && index < sideLength * 2) return 1;
	if (index >= sideLength * 2 && index < sideLength * 3) return 2;
	if (index >= sideLength * 3 && index < sideLength * 4) return 3;

	return 0;
}

void Gameplay::drawPieceToLocation(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;

	for (int i = 0; i < piece.size(); i++) {
		if (piece[i]) {
			index = (y + hackyIndexGetter(i)) * display.GetWidth() + (x + (i % sideLength));
			displayBuffer[index] = 'X';
		}
	}
}

bool Gameplay::doesPieceFit(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;
	uint32_t leftBound = 0;
	uint32_t rightBound = 0;

	for (int i = 0; i < piece.size(); i++) {
		if (piece[i] != 0) {
			index = (y + hackyIndexGetter(i)) * display.GetWidth() + (x + (i % sideLength));
			leftBound = (hackyIndexGetter(i) + y) * display.GetWidth();
			rightBound = (hackyIndexGetter(i) + y) * display.GetWidth() + (display.GetWidth() - 1);
			if ((index >= leftBound)
				&& (index < rightBound)
				&& (index < (display.GetHeight() * display.GetWidth()))
				&& (index >= 0)
				)
			{

			}
			else {
				return false;
			}
		}
	}
	return true;
}