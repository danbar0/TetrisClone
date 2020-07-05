#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include "IState.hpp"
#include <array>
#include <unordered_map>
#include <vector>
#include <memory>

class Tetris
{
public:
	enum class State {
		MAIN_MENU,
		GAMEPLAY,
		GAME_OVER,

		TOTAL_STATES
	};
	using timeTickDelayFunc = void(*) (void);
	using stateDict = std::map<State, std::shared_ptr<IState>>;

	Tetris(IPlayerInput&, IPlayingField&, timeTickDelayFunc);
	~Tetris(); 

	void Run(); 

private: 
	IPlayerInput& playerInput; 
	IPlayingField& display; 
	timeTickDelayFunc delay; 
	stateDict states; 
	std::shared_ptr<IState> currentState;
	IPlayingField::Buffer outputBuffer; 
	uint32_t timeTicks; 
	IPlayerInput::inputs keys; 

	void updateInputs(); 
	void updateState();
	void updateTime();
	void updateDisplay(); 

};

