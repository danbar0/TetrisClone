#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include <array>
#include <map>
#include <vector>

class Tetris
{
public:
	static constexpr uint8_t sideLength = 4; 
	using Piece = std::array<bool, sideLength * sideLength>;
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

	Piece rotatePiece(Piece);
	uint8_t hackyIndexGetter(uint8_t);
	void drawPieceToLocation(Piece, uint32_t, uint32_t);
	bool doesPieceFit(Piece, uint32_t, uint32_t); 
	void clearDisplayBuffer(); 
};

