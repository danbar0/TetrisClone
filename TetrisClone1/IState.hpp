#pragma once
#include <array>
#include "IPlayerInput.h"

class IState {
public:
	using currentTime = uint16_t; 

	~IState() {}
	virtual void Setup() = 0;
	virtual void Teardown() = 0;
	virtual void Update(IPlayingField::buffer&, IPlayerInput::inputs, currentTime) = 0; 
};