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
	static constexpr uint8_t defaultDifficulty = 10;
	static constexpr uint8_t sideLength = 4;
	struct Piece {
		std::array<bool, sideLength* sideLength> shape;
		char displayCharacter; 
		uint32_t x_pos;
		uint32_t y_pos;
	};
	using timeTickDelayFunc = void(*) (void);

	class GameplayState {
	public:
		GameplayState(Gameplay& game) : game(game) {}
		virtual void Update(IPlayingField::buffer&, IPlayerInput::inputs) = 0;
	protected:
		Gameplay& game; 
	};
	class PieceFalling : public GameplayState {
	public:
		PieceFalling(Gameplay& game) : GameplayState(game) {}
		void Update(IPlayingField::buffer&, IPlayerInput::inputs) override;
	};
	class SetPiece : public GameplayState {
	public:
		SetPiece(Gameplay& game) : GameplayState(game) {}
		void Update(IPlayingField::buffer&, IPlayerInput::inputs) override;
	};
	class ClearingLines : public GameplayState {
	public:
		ClearingLines(Gameplay& game) : GameplayState(game) {}
		void Update(IPlayingField::buffer&, IPlayerInput::inputs) override;
	};

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
	uint32_t displayWidth;
	uint32_t displayHeight;

	PieceFalling pieceFalling;
	SetPiece setPiece; 
	ClearingLines clearingLines;

	IPlayingField::buffer fieldData;
	std::vector<bool> completedLineIndex;

	GameplayState* currentState;
	std::map<PieceName, Piece> pieces;
	uint32_t displayCenter;
	
	uint32_t difficulty;
	Piece currentPiece;
	bool rotationLock;
	IState::currentTime currentTime; 
	uint32_t clearedLines; 

	bool doesPieceFit(Piece, uint32_t, uint32_t);
	bool linesNeedToBeCleared();
	void handleStates(IPlayerInput::inputs);
	void updateNormal(IPlayerInput::inputs);
	void updateClearingLines(); 
	void resetToNewPiece();
	void assignPieceToField(Piece); 
	void drawPieceToLocation(IPlayingField::buffer&, Piece);
	void clearDisplayBuffer(IPlayingField::buffer&);
	Piece getRandomPiece();
	Piece rotatePiece(Piece);
	uint8_t hackyIndexGetter(uint8_t);
};


