#pragma once
#include "IPlayerInput.h"
class KeyboardInput : public IPlayerInput
{
public:
	KeyboardInput();
	~KeyboardInput();

	inputs GetPlayerInputs() override; 

private:
	inputs inputs; 
};

