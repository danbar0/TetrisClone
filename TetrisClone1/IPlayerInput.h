#pragma once
#include<map>

class IPlayerInput
{
public:
	enum class Command {
		LEFT,
		RIGHT,
		UP,
		DOWN,
		SPACE
	};

	using inputs = std::map<Command, bool>;

	virtual ~IPlayerInput() {}
	virtual inputs GetPlayerInputs() = 0; 
};

