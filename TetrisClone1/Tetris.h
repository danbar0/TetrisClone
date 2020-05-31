#pragma once
#include "IPlayerInput.h"
#include "IPlayingField.h"
#include <array>
#include <vector>

class Tetris
{
public:
	using Tetronimo = std::array<bool, 16>; 

	Tetris(IPlayerInput&, IPlayingField&, std::vector<Tetronimo>&);
	~Tetris(); 

	void Begin(); 

private:
	IPlayerInput& input;
	IPlayingField& display; 
	std::vector<Tetronimo> pieces; 
};

