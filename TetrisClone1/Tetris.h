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
	using timeTickDelayFunc = void(*) (void);

	enum class PieceName {
		Square,
		Line,
		RightZee,
		LeftZee,
		Tee,
		LeftBend,
		RightBend
	};

	Tetris(IPlayerInput&, IPlayingField&, timeTickDelayFunc);
	~Tetris(); 

	void Run(); 

private:
	IPlayerInput& input;
	IPlayingField& display; 
	timeTickDelayFunc delay; 
	std::map<PieceName, Piece> pieces;
	std::vector<uint8_t> displayBuffer; 
};

