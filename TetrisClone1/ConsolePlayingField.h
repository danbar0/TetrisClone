#pragma once
#include "IPlayingField.h"
#include <Windows.h>
#include <thread>
#include <vector>

class ConsolePlayingField : public IPlayingField
{
public:
	using displayType = wchar_t; 

	ConsolePlayingField(int screenWidth = STANDARD_WIDTH, int screenHeight = STANDARD_HEIGHT);
	ConsolePlayingField() = delete; 
	~ConsolePlayingField();

	void Draw() override;
	void UpdateDisplayBuffer(std::vector<uint8_t>) override;
	void UpdateScore(uint32_t) override; 

	uint32_t GetHeight() { return screenHeight; }
	uint32_t GetWidth() { return screenWidth; }

private:
	uint32_t screenWidth;
	uint32_t screenHeight;
	uint32_t score; 

	uint32_t screenArea; 
	displayType* displayBuffer;
	HANDLE Console;
	DWORD bytesWritten; 
};

