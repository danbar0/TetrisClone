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


Gameplay::Gameplay(uint32_t width, uint32_t height) :
	displayWidth(width),
	displayHeight(height)
{
	pieces[PieceName::Square] = square;
	pieces[PieceName::Line] = line;
	pieces[PieceName::RightZee] = rightZee;
	pieces[PieceName::LeftZee] = leftZee;
	pieces[PieceName::Tee] = tee;
	pieces[PieceName::LeftBend] = leftBend;
	pieces[PieceName::RightBend] = rightBend;
}

Gameplay::~Gameplay()
{

}

void Gameplay::Setup() {

}

void Gameplay::Teardown() {

}

void Gameplay::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs, IState::currentTime time) {
	uint32_t difficulty = 1000;
	auto currentPiece = pieces[PieceName::Line];
	bool rotationLock = false;
	int x = 0, y = 0;

	if (time % difficulty == 0) {
		y += (doesPieceFit(currentPiece, x, y + 1)) ? 1 : 0;
	}


	if (inputs[IPlayerInput::Command::SPACE]) {
		if (doesPieceFit(rotatePiece(currentPiece), x, y) && !rotationLock) {
			currentPiece = rotatePiece(currentPiece);
			rotationLock = true;
		}
	}
	else {
		rotationLock = false;
	}

	x += (inputs[IPlayerInput::Command::RIGHT] && doesPieceFit(currentPiece, x + 1, y)) ? 1 : 0;
	x -= (inputs[IPlayerInput::Command::LEFT] && doesPieceFit(currentPiece, x - 1, y)) ? 1 : 0;
	//y += (inputs[Command::DOWN] && doesPieceFit(currentPiece, x, y + 1)) ? 1 : 0; 


	// Display 
	clearDisplayBuffer();
	drawPieceToLocation(buffer, currentPiece, x, y);
	
}

void Gameplay::clearDisplayBuffer() {
	std::fill(displayBuffer.begin(), displayBuffer.end(), 0);
}

Gameplay::Piece Gameplay::rotatePiece(Piece piece)
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

void Gameplay::drawPieceToLocation(IPlayingField::buffer& displayBuffer, Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;

	for (int i = 0; i < piece.size(); i++) {
		if (piece[i]) {
			index = (y + hackyIndexGetter(i)) * displayWidth + (x + (i % sideLength));
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
			index = (y + hackyIndexGetter(i)) * displayWidth + (x + (i % sideLength));
			leftBound = (hackyIndexGetter(i) + y) * displayWidth;
			rightBound = (hackyIndexGetter(i) + y) * displayWidth + (displayWidth - 1);
			if ((index >= leftBound)
				&& (index < rightBound)
				&& (index < (displayHeight * displayWidth))
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