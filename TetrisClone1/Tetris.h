#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include <array>
#include <map>
#include <vector>

class Tetris
{
public:
	static constexpr uint8_t pieceDimension = 4; 
	using Piece = std::array<bool, pieceDimension*pieceDimension>;
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

	enum class RotationDirection {
		LEFT,
		RIGHT
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

	Piece getRotatedPiece(Piece, RotationDirection);
};

