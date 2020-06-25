#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include "IState.hpp"
#include <array>
#include <unordered_map>
#include <vector>

class Tetris
{
public:
	enum class State {
		Game,

		TOTAL_STATES
	};
	using timeTickDelayFunc = void(*) (void);
	using stateDict = std::unordered_map<State, IState*>;

	Tetris(IPlayerInput&, IPlayingField&, timeTickDelayFunc);
	~Tetris(); 

	void Run(); 

private: 
	IPlayerInput& playerInput; 
	IPlayingField& display; 
	timeTickDelayFunc delay; 
	stateDict states; 
	IState* currentState; 
	std::vector<uint8_t> displayBuffer; 
	uint32_t timeTicks; 
	IPlayerInput::inputs keys; 

	void updateInputs(); 
	void updateState();
	void updateTime();
	void updateDisplay(); 

};

