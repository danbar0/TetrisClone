#include "pch.h"
#include "Gameplay.h"
#include "Tetris.h"
#include <math.h>
#include <time.h>
#include <random>


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

/*-----------------------------------------------------------------------------------------------*/
Gameplay::Gameplay(uint32_t width, uint32_t height) :
	displayWidth(width),
	displayHeight(height),
	pieceFalling(*this),
	setPiece(*this),
	clearingLines(*this),
	completedLineIndex(displayHeight, false)
{
	gameData.field = *new IPlayingField::field(static_cast<uint64_t>(width) * static_cast<uint64_t>(height));
	inactivePieceBuffer = *new IPlayingField::field(static_cast<uint64_t>(width) * static_cast<uint64_t>(height));
	activePieceBuffer = *new IPlayingField::field(static_cast<uint64_t>(width) * static_cast<uint64_t>(height));
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

/*-----------------------------------------------------------------------------------------------*/
Gameplay::~Gameplay()
{

}

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::Setup() {
	currentState = &pieceFalling;
	isDone = false; 
	currentPiece = getRandomPiece();
	currentPiece.x_pos = displayCenter;
	currentPiece.y_pos = 0;
	difficulty = defaultDifficulty;
	rotationLock = false;
	dropLock = false; 
	clearedLines = 0; 
} 
 
/*-----------------------------------------------------------------------------------------------*/
void Gameplay::Teardown() { 
	std::fill(gameData.field.begin(), gameData.field.end(), 0); 
	std::fill(inactivePieceBuffer.begin(), inactivePieceBuffer.end(), 0);
	std::fill(activePieceBuffer.begin(), activePieceBuffer.end(), 0);
	std::fill(completedLineIndex.begin(), completedLineIndex.end(), 0);
} 
 
/*-----------------------------------------------------------------------------------------------*/
void Gameplay::Update(IPlayingField::Buffer& buffer, IPlayerInput::inputs inputs, IState::currentTime time) { 
	currentTime = time; 
	currentState->Update(inputs); 
	drawToBuffer(buffer); 
} 

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::drawToBuffer(IPlayingField::Buffer& buffer) {
	std::fill(buffer.field.begin(), buffer.field.end(), 0);

	for (int i = 0; i < buffer.field.size(); i++) {
		if (activePieceBuffer[i]) buffer.field[i] = activePieceBuffer[i];
		if (inactivePieceBuffer[i]) buffer.field[i] = inactivePieceBuffer[i]; 
	}

	buffer.clearedLines = clearedLines; 
}
 
/*-----------------------------------------------------------------------------------------------*/
Gameplay::Piece Gameplay::getRandomPiece() {
	std::random_device rd; 

	auto it = pieces.begin(); 
	std::advance(it, rd() % pieces.size());
	return it->second; 
}

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::resetToNewPiece() {
	currentPiece = getRandomPiece();
	currentPiece.x_pos = displayCenter;
	currentPiece.y_pos = 0;

	if (!doesPieceFit(currentPiece, currentPiece.x_pos, currentPiece.y_pos)) {
		this->isDone = true; 
	}
}

/*-----------------------------------------------------------------------------------------------*/
Gameplay::Piece Gameplay::rotatePiece(Piece piece)
{
	Gameplay::Piece rotatedPiece{ {0}, piece.displayCharacter , piece.x_pos, piece.y_pos};
	int index;

	for (int i = 0; i < piece.shape.size(); i++) {
		index = sideLength - 1 - yIndexOffset(i) + ((i % sideLength) * sideLength);
		rotatedPiece.shape[i] = piece.shape[index];
	}

	return rotatedPiece;
}

/*-----------------------------------------------------------------------------------------------*/
uint8_t Gameplay::yIndexOffset(uint8_t index) {
	if (index >= 0 && index < sideLength) return 0;
	if (index >= sideLength && index < sideLength * 2) return 1;
	if (index >= sideLength * 2 && index < sideLength * 3) return 2;
	if (index >= sideLength * 3 && index < sideLength * 4) return 3;

	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::assignPieceToField(Piece piece) {
	uint32_t index = 0;
	updatePieceLocation(currentPiece);

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i]) {
			index = (piece.y_pos + yIndexOffset(i)) * displayWidth + (piece.x_pos + (i % sideLength));
			activePieceBuffer[index] = 0; 
			inactivePieceBuffer[index] = piece.displayCharacter;
		}
	}
}

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::updatePieceLocation(Piece piece) {
	uint32_t index = 0;
	std::fill(activePieceBuffer.begin(), activePieceBuffer.end(), 0);

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i]) {
			index = (piece.y_pos + yIndexOffset(i)) * displayWidth + (piece.x_pos + (i % sideLength));
			activePieceBuffer[index] = piece.displayCharacter;
		}
	}
}

/*-----------------------------------------------------------------------------------------------*/
bool Gameplay::doesPieceFit(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0;
	uint32_t leftBound = 0;
	uint32_t rightBound = 0;

	for (int i = 0; i < piece.shape.size(); i++) {
		if (piece.shape[i] != 0) {
			index = (y + yIndexOffset(i)) * displayWidth + (x + (i % sideLength));
			leftBound = (yIndexOffset(i) + y) * displayWidth;
			rightBound = (yIndexOffset(i) + y) * displayWidth + (displayWidth);

			if (!((index >= leftBound)
				&& (index < rightBound)
				&& (index < (displayHeight * displayWidth))
				&& (inactivePieceBuffer[index] == 0))
				)
			{
				return false; 
			}
		}
	}

	return true;
}

/*-----------------------------------------------------------------------------------------------*/
bool Gameplay::linesNeedToBeCleared() {
	uint32_t index = 0; 

	for (int i = 0; i < displayHeight; i++) {
		index = i * displayWidth;

		// Check every index to the right of the first cell in the leftmost column that is filled
		if (inactivePieceBuffer[index] != 0) {
			for (int j = 1; j < displayWidth; j++) {
				if (inactivePieceBuffer[index + j] == 0) {
					completedLineIndex[i] = false;
					break; 
				}

				completedLineIndex[i] = true;
			}
		}
	}

	return (find(completedLineIndex.begin(), completedLineIndex.end(), true) 
			!= completedLineIndex.end());
}


/*-----------------------------------------------------------------------------------------------*/
void Gameplay::PieceFalling::Update(IPlayerInput::inputs inputs) {
	static bool rightLock = false;
	static bool leftLock = false;
	if (inputs[IPlayerInput::Command::UP]) {
		if (!game.dropLock) {
			game.dropLock = true;
			game.currentState = &game.setPiece;
			return;
		}
	}
	else {
		game.dropLock = false;
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

	if (inputs[IPlayerInput::Command::RIGHT]) {
		static uint32_t rightHoldTimer = 0;
		if (game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos + 1, game.currentPiece.y_pos) 
			&& !rightLock) 
		{
			game.currentPiece.x_pos += 1;
			rightLock = true; 
			rightHoldTimer = game.currentTime;
		}
		if (game.currentTime - rightHoldTimer > game.holdTime 
			&& game.currentTime % game.pieceMovementTimer == 0
			&& game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos + 1, game.currentPiece.y_pos)) 
		{
			game.currentPiece.x_pos += 1;
		}

	}
	else {
		rightLock = false; 
	}

	if (inputs[IPlayerInput::Command::LEFT]) {
		static uint32_t leftHoldTimer = 0; 
		if (game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos - 1, game.currentPiece.y_pos) && !leftLock) {
			game.currentPiece.x_pos -= 1;
			leftLock = true; 
			leftHoldTimer = game.currentTime;
		}
		if (game.currentTime - leftHoldTimer > game.holdTime 
			&& game.currentTime % game.pieceMovementTimer == 0
			&& game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos - 1, game.currentPiece.y_pos)) 
		{
			game.currentPiece.x_pos -= 1;
		}

	}
	else {
		leftLock = false; 
	}


	if(inputs[IPlayerInput::Command::DOWN]) {
		if (game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos, game.currentPiece.y_pos + 1)
			&& game.currentTime % game.pieceMovementTimer == 0)
		{
			game.currentPiece.y_pos += 1;
		}
	}

	else if (game.currentTime % game.difficulty == 0) {
		if (game.doesPieceFit(game.currentPiece, game.currentPiece.x_pos, game.currentPiece.y_pos + 1)) {
			game.currentPiece.y_pos += 1;
		}

		else {
			game.assignPieceToField(game.currentPiece);

			if (game.linesNeedToBeCleared()) {
				game.currentState = &game.clearingLines;
				return; 
			}
			else {
				game.resetToNewPiece();
			}
		}
	}

	game.updatePieceLocation(game.currentPiece);
}

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::SetPiece::Update(IPlayerInput::inputs inputs) {

	for (int i = 0; i < game.displayHeight; i++) {
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

/*-----------------------------------------------------------------------------------------------*/
void Gameplay::ClearingLines::Update(IPlayerInput::inputs inputs) {

	for (int i = 0; i < game.completedLineIndex.size(); i++) {
		if (game.completedLineIndex[i]) {
			std::fill((game.inactivePieceBuffer.begin()+(i * game.displayWidth)), (game.inactivePieceBuffer.begin()+(i * game.displayWidth) + game.displayWidth), 0);
			std::rotate(game.inactivePieceBuffer.begin(),
				game.inactivePieceBuffer.begin() + (i * game.displayWidth),
				game.inactivePieceBuffer.begin() + ((i * game.displayWidth) + game.displayWidth));

			game.completedLineIndex[i] = false; 

			game.clearedLines++;
			if (game.difficulty > 1) {
				game.difficulty -= 1; 
			}
		}
	}

	game.resetToNewPiece();
	game.currentState = &game.pieceFalling;
}