#include "pch.h"
#include "Gameplay.h"
#include "Tetris.h"
#include <math.h>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

Gameplay::Piece test{
	{
	1,1,1,1,
	1,1,1,1,
	1,1,1,1,
	1,1,1,1
	},
	'X'
};

Gameplay::Piece square{
	{
	0,0,0,0,
	0,1,1,0,
	0,1,1,0,
	0,0,0,0
	},
	'A'
};

Gameplay::Piece line{
	{
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,1,0,0
	},
	'B'
};

Gameplay::Piece rightZee{
	{
	0,1,0,0,
	0,1,1,0,
	0,0,1,0,
	0,0,0,0
	},
	'C'
};

Gameplay::Piece leftZee{
	{
	0,0,1,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
	},
	'D'
};

Gameplay::Piece tee{
	{
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0,
	},
	'E'
};

Gameplay::Piece leftBend{
	{
	0,1,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,0,0
	},
	'F'
};

Gameplay::Piece rightBend{
	{
	0,1,1,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0
	},
	'G'
};


Gameplay::Gameplay(uint32_t width, uint32_t height) :
	displayWidth(width),
	displayHeight(height),
	normal(*this),
	clearingLines(*this),
	fieldData(static_cast<uint64_t>(width) * static_cast<uint64_t>(height)),
	completedLineIndex(displayHeight, false)
{
	pieces[PieceName::Square] = square;
	pieces[PieceName::Line] = line;
	pieces[PieceName::RightZee] = rightZee;
	pieces[PieceName::LeftZee] = leftZee;
	pieces[PieceName::Tee] = tee;
	pieces[PieceName::LeftBend] = leftBend;
	pieces[PieceName::RightBend] = rightBend;

	displayCenter = round((displayWidth / 2) - 1);
	Setup();
}

Gameplay::~Gameplay()
{

}

void Gameplay::Setup() {
	currentState = &normal; 
	isDone = false; 
	piece_x = displayCenter;
	piece_y = 0;
	difficulty = 10;
	currentPiece = getRandomPiece();
	rotationLock = false;
} 
 
void Gameplay::Teardown() { 
	std::fill(fieldData.begin(), fieldData.end(), 0); 
	std::fill(completedLineIndex.begin(), completedLineIndex.end(), 0);
} 
 
void Gameplay::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs, IState::currentTime time) { 
	currentTime = time; 
	currentState->Update(buffer, inputs); 
} 
 
Gameplay::Piece Gameplay::getRandomPiece() {
	auto it = pieces.begin(); 
	std::advance(it, rand() % pieces.size());
	return it->second; 
}

void Gameplay::resetToNewPiece() {
	currentPiece = getRandomPiece();
	piece_x = displayCenter;
	piece_y = 0;

	if (!doesPieceFit(currentPiece, piece_x, piece_y)) {
		this->isDone = true; 
	}
}

void Gameplay::assignPieceToField(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;  

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i]) {
			index = (y + hackyIndexGetter(i)) * displayWidth + (x + (i % sideLength));
			fieldData[index] = piece.displayCharacter; 
		}
	}
}

void Gameplay::clearDisplayBuffer(IPlayingField::buffer& buffer) {
	std::fill(buffer.begin(), buffer.end(), 0);
}

Gameplay::Piece Gameplay::rotatePiece(Piece piece)
{
	Gameplay::Piece rotatedPiece{ {0}, piece.displayCharacter };
	int index;

	for (int i = 0; i < piece.shape.size(); i++) {
		index = sideLength - 1 - hackyIndexGetter(i) + ((i % sideLength) * sideLength);
		rotatedPiece.shape[i] = piece.shape[index];
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

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i]) {
			index = (y + hackyIndexGetter(i)) * displayWidth + (x + (i % sideLength));
			displayBuffer[index] = piece.displayCharacter;
		}
	}
}

bool Gameplay::doesPieceFit(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;
	uint32_t leftBound = 0;
	uint32_t rightBound = 0;

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i] != 0) {
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

bool Gameplay::linesNeedToBeCleared() {
	uint32_t index = 0; 

	for (int i = 0; i < displayHeight; i++) {
		index = i * displayWidth;

		if (fieldData[index] != 0) {
			for (int j = 1; j < displayWidth; j++) {
				if (fieldData[index + j] == 0) {
					completedLineIndex[i] = false;
					break; 
				}

				completedLineIndex[i] = true;
			}
		}
	}

	return (find(completedLineIndex.begin(), completedLineIndex.end(), true) != completedLineIndex.end());
}

void Gameplay::Normal::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs) {
	if (inputs[IPlayerInput::Command::SPACE]) {
		if (game.doesPieceFit(game.rotatePiece(game.currentPiece), game.piece_x, game.piece_y) && !game.rotationLock) {
			game.currentPiece = game.rotatePiece(game.currentPiece);
			game.rotationLock = true;
		}
	}
	else {
		game.rotationLock = false;
	}

	game.piece_x += (inputs[IPlayerInput::Command::RIGHT] && game.doesPieceFit(game.currentPiece, game.piece_x + 1, game.piece_y)) ? 1 : 0;
	game.piece_x -= (inputs[IPlayerInput::Command::LEFT] && game.doesPieceFit(game.currentPiece, game.piece_x - 1, game.piece_y)) ? 1 : 0;
	game.piece_y += (inputs[IPlayerInput::Command::DOWN] && game.doesPieceFit(game.currentPiece, game.piece_x, game.piece_y + 1)) ? 1 : 0;

	if (game.currentTime % game.difficulty == 0) {
		if (game.doesPieceFit(game.currentPiece, game.piece_x, game.piece_y + 1)) {
			game.piece_y += 1;
		}

		else {
			game.assignPieceToField(game.currentPiece, game.piece_x, game.piece_y);

			if (game.linesNeedToBeCleared()) {
				game.currentState = &game.clearingLines;
			}
			else {
				game.resetToNewPiece();
			}
		}
	}

	game.clearDisplayBuffer(buffer);
	game.drawPieceToLocation(buffer, game.currentPiece, game.piece_x, game.piece_y);
}

void Gameplay::ClearingLines::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs) {
	uint32_t index; 

	for (int i = 0; i < game.completedLineIndex.size(); i++) {
		if (game.completedLineIndex[i]) {
			std::fill((game.fieldData.begin()+(i * game.displayWidth)), (game.fieldData.begin()+(i * game.displayWidth) + game.displayWidth), 0);
			std::rotate(game.fieldData.begin(),
				game.fieldData.begin() + (i * game.displayWidth),
				game.fieldData.begin() + ((i * game.displayWidth) + game.displayWidth));

			game.completedLineIndex[i] = false; 
		}
	}

	game.resetToNewPiece();
	game.currentState = &game.normal;
}