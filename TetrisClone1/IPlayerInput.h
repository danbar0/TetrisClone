#pragma once
#include<map>

enum class Command {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	SPACE
};

using inputs = std::map<Command, bool>;

class IPlayerInput
{
public:

	virtual ~IPlayerInput() {}
	virtual inputs GetPlayerInputs() = 0; 
};

