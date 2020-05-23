#pragma once
#include<map>

enum class Commands {
	Left,
	Right,
	Up,
	Down,
	Space
};

using inputs = std::map<Commands, bool>;

class IPlayerInput
{
public:

	virtual ~IPlayerInput() {}
	virtual inputs GetPlayerInputs() = 0; 
};

