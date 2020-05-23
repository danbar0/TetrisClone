#include "pch.h"
#include "ConsolePlayingField.h"


ConsolePlayingField::ConsolePlayingField(int screenWidth, int screenHeight) :
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	displayBuffer = std::make_unique<wchar_t[]>(screenWidth*screenHeight);
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CONSOLE_TEXTMODE_BUFFER, 
		NULL);
	bytesWritten = 0; 

	for (int i = 0; i < screenWidth*screenHeight; i++) {
		displayBuffer[i] = ' ';
	}
}


ConsolePlayingField::~ConsolePlayingField()
{

}
