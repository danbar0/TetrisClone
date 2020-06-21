#pragma once
#include <array>
#include "IPlayerInput.h"

class IState {
public:
	using currentTime = uint16_t; 

	~IState() {}
	virtual void setup() = 0;
	virtual void teardown() = 0;
	virtual void update(IPlayingField::buffer, IPlayerInput::inputs, currentTime) = 0; 
};
