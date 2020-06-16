#include "pch.h"
#include "Tetris.h"

Tetris::Piece test{
	1,1,1,1,
	1,1,1,1,
	1,1,1,1,
	1,1,1,1
};

Tetris::Piece square{
	0,0,0,0,
	0,1,1,0,
	0,1,1,0,
	0,0,0,0
};

Tetris::Piece line{
	0,0,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,1,0
};

Tetris::Piece rightZee{
	0,1,0,0,
	0,1,1,0,
	0,0,1,0,
	0,0,0,0
};

Tetris::Piece leftZee {
	0,0,1,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

Tetris::Piece tee {
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0,
};

Tetris::Piece leftBend{
	0,1,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,0,0
};

Tetris::Piece rightBend{
	0,1,1,0,
	0,1,0,0,
	0,1,0,0,
	0,0,0,0
};


Tetris::Tetris(IPlayerInput& input, IPlayingField& display, timeTickDelayFunc delay) :
	input(input),
	display(display),
	delay(delay)
{
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

Tetris::~Tetris() 
{
	
}

void Tetris::Run()
{
	auto currentPiece = pieces[PieceName::Tee];
	int x = 0, y = 0; 

	while(1) {
		delay();
		auto inputs = input.GetPlayerInputs();

		if (inputs[Command::SPACE]) {
			currentPiece = rotatePiece(currentPiece);
		}
		if (inputs[Command::RIGHT]) x++;
		if (inputs[Command::LEFT]) x--;

		drawPieceToLocation(currentPiece, x, y);


		display.UpdateDisplayBuffer(displayBuffer);
		display.Draw();
	}
}

Tetris::Piece Tetris::rotatePiece(Piece piece)
{
	Tetris::Piece rotatedPiece {0};
	int index; 

	for (int i = 0; i < piece.size(); i++) {
		index = sideLength - 1 - hackyIndexGetter(i) + ((i % sideLength) * sideLength);
		rotatedPiece[i] = piece[index];
	}

	return rotatedPiece;
}

uint8_t Tetris::hackyIndexGetter(uint8_t index) {
	if (index >= 0 && index <= sideLength-1) return 0;
	if (index >= sideLength && index <= sideLength*2-1) return 1;
	if (index >= sideLength*2 && index <= sideLength*3-1) return 2;
	if (index >= sideLength*3 && index <= sideLength*4-1) return 3;

	return 0;
}

void Tetris::drawPieceToLocation(Piece piece, uint32_t x, uint32_t y) {
	uint32_t index = 0; 

	for (int i = 0; i < piece.size(); i++) {
		index = (y + hackyIndexGetter(i)) * display.GetWidth() + (x + (i % sideLength));
		if (piece[i]) {
			displayBuffer[index] = 'X';
		}
		else {
			displayBuffer[index] = ' ';
		}
	}
}