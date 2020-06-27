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
	displayHeight(height),
	fieldData(width*height),
	piece_x(0),
	piece_y(0),
	difficulty(10),
	currentPiece(line),
	rotationLock(false)
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
	isDone = false; 
	piece_x = 0;
	piece_y = 0;
	difficulty = 10;
	currentPiece = getRandomPiece();
	rotationLock = false;
}

void Gameplay::Teardown() {
	std::fill(fieldData.begin(), fieldData.end(), 0);
}

void Gameplay::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs, IState::currentTime time) {
	if (inputs[IPlayerInput::Command::SPACE]) {
		if (doesPieceFit(rotatePiece(currentPiece), piece_x, piece_y) && !rotationLock) {
			currentPiece = rotatePiece(currentPiece);
			rotationLock = true;
		}
	}
	else {
		rotationLock = false;
	}

	piece_x += (inputs[IPlayerInput::Command::RIGHT] && doesPieceFit(currentPiece, piece_x + 1, piece_y)) ? 1 : 0;
	piece_x -= (inputs[IPlayerInput::Command::LEFT] && doesPieceFit(currentPiece, piece_x - 1, piece_y)) ? 1 : 0;
	piece_y += (inputs[IPlayerInput::Command::DOWN] && doesPieceFit(currentPiece, piece_x, piece_y + 1)) ? 1 : 0;

	if (time % difficulty == 0) {
		if (doesPieceFit(currentPiece, piece_x, piece_y + 1)) {
			piece_y += 1;
		}
		
		else {
			assignPieceToField(currentPiece, piece_x, piece_y);
			resetToNewPiece();
		}
	}

	clearDisplayBuffer(buffer);
	drawPieceToLocation(buffer, currentPiece, piece_x, piece_y);
	
}

Gameplay::Piece Gameplay::getRandomPiece() {
	auto it = pieces.begin(); 
	std::advance(it, rand() % pieces.size());
	return it->second; 
}

void Gameplay::resetToNewPiece() {
	currentPiece = getRandomPiece();
	piece_x = 0;
	piece_y = 0;

	if (!doesPieceFit(currentPiece, piece_x, piece_y)) {
		this->isDone = true; 
	}
}

void Gameplay::assignPieceToField(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;  

	for (int i = 0; i < piece.size(); i++) {
		if (piece[i]) {
			index = (y + hackyIndexGetter(i)) * displayWidth + (x + (i % sideLength));
			fieldData[index] = 'X'; 
		}
	}
}

void Gameplay::clearDisplayBuffer(IPlayingField::buffer& buffer) {
	std::fill(buffer.begin(), buffer.end(), 0);
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
	displayBuffer = fieldData; 

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
			rightBound = (hackyIndexGetter(i) + y) * displayWidth + (displayWidth);

			if (!((index >= leftBound)
				&& (index < rightBound)
				&& (index < (displayHeight * displayWidth))
				&& (index >= 0)
				&& (fieldData[index] == 0))
				)
			{
				return false; 
			}
		}
	}
	return true;
}