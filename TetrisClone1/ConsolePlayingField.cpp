#include "pch.h"
#include "ConsolePlayingField.h"


ConsolePlayingField::ConsolePlayingField(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	bytesWritten = 0;
	displayBuffer = std::make_unique<wchar_t[]>(screenWidth*screenHeight);
	Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CONSOLE_TEXTMODE_BUFFER, 
		NULL);

	for (int i = 0; i < screenWidth*screenHeight; i++) {
		displayBuffer[i] = ' ';
	}

}


ConsolePlayingField::~ConsolePlayingField()
{

}

void ConsolePlayingField::UpdateDisplay(uint32_t* buffer) 
{
	
}

void ConsolePlayingField::UpdateScore(uint32_t) 
{

}
