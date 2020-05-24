#pragma once
#include "IPlayingField.h"
#include <Windows.h>
#include <thread>

class ConsolePlayingField : public IPlayingField
{
public:
	ConsolePlayingField(int screenWidth = STANDARD_WIDTH, int screenHeight = STANDARD_HEIGHT);
	~ConsolePlayingField();

	void UpdateDisplay(uint32_t*) override; 
	void UpdateScore(uint32_t) override; 

private:
	int screenWidth;
	int screenHeight;

	std::unique_ptr<wchar_t[]> displayBuffer;
	HANDLE Console;
	DWORD bytesWritten; 
};

