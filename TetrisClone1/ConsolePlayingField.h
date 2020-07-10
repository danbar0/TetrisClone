#pragma once
#include "IPlayingField.h"
#include <Windows.h>
#include <thread>
#include <vector>
#include <map>
#include <string>

class ConsolePlayingField : public IPlayingField
{
public:
	using displayType = CHAR_INFO; 

	ConsolePlayingField(int screenWidth = STANDARD_WIDTH, int screenHeight = STANDARD_HEIGHT);
	ConsolePlayingField() = delete; 
	~ConsolePlayingField();

	void Draw() override;
	void UpdateDisplayBuffer(Buffer&) override;

	uint32_t GetHeight() { return screenHeight; }
	uint32_t GetWidth() { return screenWidth; }

private:
	uint32_t screenWidth;
	uint32_t screenHeight;
	std::wstring score; 
	uint32_t scoreLength; 

	std::map<blockType, uint32_t> attributeMap; 
	uint32_t screenArea; 
	displayType* displayBuffer;
	HANDLE Console;
	DWORD bytesWritten; 

	std::wstring s2ws(const std::string& s); 
};

