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


Tetris::Tetris(IPlayerInput& input, IPlayingField& display) :
	input(input),
	display(display)
{
	pieces[PieceName::Square] = square;
	pieces[PieceName::Line] = line;
	pieces[PieceName::RightZee] = rightZee;
	pieces[PieceName::LeftZee] = leftZee;
	pieces[PieceName::Tee] = tee;
	pieces[PieceName::LeftBend] = leftBend;
	pieces[PieceName::RightBend] = rightBend;
}

Tetris::~Tetris() 
{

}

void Tetris::Run() 
{

}