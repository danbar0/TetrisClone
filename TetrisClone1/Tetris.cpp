#include "pch.h"
#include "Tetris.h"

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

Tetris::Piece leftZee{
	0,0,1,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
};

Tetris::Piece tee{
	0,1,0,0,
	0,1,1,0,
	0,1,0,0,
	0,0,0,0
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
	delay(); 
	auto inputs = input.GetPlayerInputs(); 


	display.UpdateDisplayBuffer(displayBuffer); 
	display.Draw(); 
	while (1); 
}

Tetris::Piece Tetris::getRotatedPiece(Piece piece, RotationDirection direction)
{
	Tetris::Piece rotatedPiece; 

	switch (direction) {
		case RotationDirection::LEFT:
			for (int i = 0; i < piece.size() / pieceDimension; i++) {

			}
			break; 

		case RotationDirection::RIGHT:
			break; 

		default:
	}

	return rotatedPiece;
}