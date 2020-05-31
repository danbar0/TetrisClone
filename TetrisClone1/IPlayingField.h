#pragma once
#include <stdint.h>

#define STANDARD_WIDTH 30
#define STANDARD_HEIGHT 80
#define PLAY_AREA STANDARD_WIDTH*STANDARD_HEIGHT

class IPlayingField
{
public:
	virtual ~IPlayingField() {};
	virtual void Draw() = 0; 
	virtual void UpdateDisplayBuffer(uint32_t*, size_t) = 0;
	virtual void UpdateScore(uint32_t) = 0; 
};

