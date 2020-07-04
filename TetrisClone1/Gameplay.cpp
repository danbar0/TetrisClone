#include "pch.h"
#include "Gameplay.h"
#include "Tetris.h"
#include <math.h>
#include <time.h>


Gameplay::Piece test{
	{
	1,1,1,1,
	1,1,1,1,
	1,1,1,1,
	1,1,1,1
	},
	'X',
	0,
	0
};

Gameplay::Piece square{
	{
	0,0,0,0,
	0,1,1,0,
	0,1,1,0,
	0,0,0,0
	},
	'A',
	0,
	0
};

Gameplay::Piece line{
	{
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,1,0,0
	},
	'B',
	0,
	0
};

Gameplay::Piece rightZee{
	{
	0,1,0,0,
	0,1,1,0,
	0,0,1,0,
	0,0,0,0
	},
	'C',
	0,
	0
};

Gameplay::Piece leftZee{
	{
	0,0,1,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
	},
	'D',
	0,
	0
};

Gameplay::Piece tee{
	{
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0,
	},
	'E',
	0,
	0
};

Gameplay::Piece leftBend{
	{
	0,1,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,0,0
	},
	'F',
	0,
	0
};

Gameplay::Piece rightBend{
	{
	0,1,1,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0
	},
	'G',
	0,
	0
};


Gameplay::Gameplay(uint32_t width, uint32_t height) :
	displayWidth(width),
	displayHeight(height),
	pieceFalling(*this),
	setPiece(*this),
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
	currentState = &pieceFalling;
	isDone = false; 
	currentPiece = getRandomPiece();
	currentPiece.x_pos = displayCenter;
	currentPiece.y_pos = 0;
	difficulty = defaultDifficulty;
	rotationLock = false;
	clearedLines = 0; 
} 
 
void Gameplay::Teardown() { 
	std::fill(fieldData.begin(), fieldData.end(), 0); 
	std::fill(completedLineIndex.begin(), completedLineIndex.end(), 0);
} 
 
void Gameplay::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs, IState::currentTime time) { 
	currentTime = time; 
	currentState->Update(buffer, inputs); 

	clearDisplayBuffer(buffer);
	drawPieceToLocation(buffer, currentPiece);
} 
 
Gameplay::Piece Gameplay::getRandomPiece() {
	srand(time(NULL));
	auto it = pieces.begin(); 
	std::advance(it, rand() % pieces.size());
	return it->second; 
}

void Gameplay::resetToNewPiece() {
	currentPiece = getRandomPiece();
	currentPiece.x_pos = displayCenter;
	currentPiece.y_pos = 0;

	if (!doesPieceFit(currentPiece, currentPiece.x_pos, currentPiece.y_pos)) {
		this->isDone = true; 
	}
}

void Gameplay::assignPieceToField(Piece piece) {
	uint32_t index = 0;  

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i]) {
			index = (piece.y_pos + hackyIndexGetter(i)) * displayWidth + (piece.x_pos + (i % sideLength));
			fieldData[index] = piece.displayCharacter; 
		}
	}
}

void Gameplay::clearDisplayBuffer(IPlayingField::buffer& buffer) {
	std::fill(buffer.begin(), buffer.end(), 0);
}

Gameplay::Piece Gameplay::rotatePiece(Piece piece)
{
	Gameplay::Piece rotatedPiece{ {0}, piece.displayCharacter , piece.x_pos, piece.y_pos};
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

void Gameplay::drawPieceToLocation(IPlayingField::buffer& displayBuffer, Piece piece) {
	uint32_t index = 0;
	displayBuffer = fieldData; 

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i]) {
			index = (piece.y_pos + hackyIndexGetter(i)) * displayWidth + (piece.x_pos + (i % sideLength));
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

void Gameplay::PieceFalling::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs) {
	if (inputs[IPlayerInput::Command::UP]) {
		game.currentState = &game.setPiece; 
	}

	if (inputs[IPlayerInput::Command::SPACE]) {
		if (game.doesPieceFit(game.rotatePiece(game.currentPiece), game.currentPiece.x_pos, game.currentPiece.y_pos) && !game.rotationLock) {
			game.currentPiece = game.rotatePiece(game.currentPiece);
			game.rotationLock = true;
		}
	}
	else {
		game.rotationLock = false;
	}

	game.currentPiece.x_pos += (inputs[IPlayerInput::Command::RIGHT] && game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos + 1, game.currentPiece.y_pos)) ? 1 : 0;
	game.currentPiece.x_pos -= (inputs[IPlayerInput::Command::LEFT] && game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos - 1, game.currentPiece.y_pos)) ? 1 : 0;
	if(inputs[IPlayerInput::Command::DOWN] && game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos, game.currentPiece.y_pos + 1)) {
		game.currentPiece.y_pos += 1;
	}

	else if (game.currentTime % game.difficulty == 0) {
		if (game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos, game.currentPiece.y_pos + 1)) {
			game.currentPiece.y_pos += 1;
		}

		else {
			game.assignPieceToField(game.currentPiece);

			if (game.linesNeedToBeCleared()) {
				game.currentState = &game.clearingLines;
			}
			else {
				game.resetToNewPiece();
			}
		}
	}
}

void Gameplay::SetPiece::Update(IPlayingField::buffer& buffer, IPlayerInput::inputs inputs) {
	for (int i = 1; i < game.displayHeight; i++) {
		if (!game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos, game.currentPiece.y_pos + 1)) {
			game.assignPieceToField(game.currentPiece);

			if (game.linesNeedToBeCleared()) {
				game.currentState = &game.clearingLines;
			}
			else {
				game.resetToNewPiece();
				game.currentState = &game.pieceFalling;
			}

			break;
		}
		else {
			game.currentPiece.y_pos += 1; 
		}
	}
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

			game.clearedLines++;
			if (game.clearedLines % 10 == 0 && game.difficulty > 1) {
				game.difficulty -= 1; 
			}
		}
	}

	game.resetToNewPiece();
	game.currentState = &game.pieceFalling;
}