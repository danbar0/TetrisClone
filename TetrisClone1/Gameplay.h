#pragma once
#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include "IState.hpp"
#include <array>
#include <map>
#include <vector>

class Gameplay : public IState
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

	Gameplay(uint32_t, uint32_t);
	~Gameplay();

	void Setup() override;
	void Teardown() override;
	void Update(IPlayingField::buffer&, IPlayerInput::inputs, IState::currentTime) override;

private:
	std::map<PieceName, Piece> pieces;
	std::vector<uint8_t> displayBuffer;
	uint32_t displayWidth;
	uint32_t displayHeight; 

	Piece rotatePiece(Piece);
	uint8_t hackyIndexGetter(uint8_t);
	void drawPieceToLocation(IPlayingField::buffer&, Piece, uint32_t, uint32_t);
	bool doesPieceFit(Piece, uint32_t, uint32_t);
	void clearDisplayBuffer();
};


