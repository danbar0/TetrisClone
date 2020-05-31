#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include <array>
#include <map>
#include <vector>

class Tetris
{
public:
	using Piece = std::array<bool, 16>; 

	enum class PieceName {
		Square,
		Line,
		RightZee,
		LeftZee,
		Tee,
		LeftBend,
		RightBend
	};

	Tetris(IPlayerInput&, IPlayingField&);
	~Tetris(); 

	void Run(); 

private:
	IPlayerInput& input;
	IPlayingField& display; 
	std::map<PieceName, Piece> pieces;
};

