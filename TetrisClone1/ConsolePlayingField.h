#pragma once
#include "IPlayingField.h"
#include <Windows.h>
#include <thread>

class ConsolePlayingField : public IPlayingField
{
public:
	using displayType = wchar_t; 

	ConsolePlayingField(int screenWidth = STANDARD_WIDTH, int screenHeight = STANDARD_HEIGHT);
	~ConsolePlayingField();

	void Draw() override;
	void UpdateDisplayData(uint32_t*) override; 
	void UpdateScore(uint32_t) override; 

private:
	int screenWidth;
	int screenHeight;

	uint32_t screenArea; 
	displayType* displayBuffer;
	HANDLE Console;
	DWORD bytesWritten; 
};

