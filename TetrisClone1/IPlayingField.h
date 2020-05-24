#pragma once

#include <array> 

#define STANDARD_WIDTH 30
#define STANDARD_HEIGHT 80
#define PLAY_AREA STANDARD_WIDTH*STANDARD_HEIGHT

class IPlayingField
{
public:
	virtual ~IPlayingField() {};
	virtual void UpdateDisplay(uint32_t*) = 0; 
	virtual void UpdateScore(uint32_t) = 0; 
};

