#include "pch.h"
#include "ConsolePlayingField.h"


ConsolePlayingField::ConsolePlayingField(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	bytesWritten = 0;
	screenArea = screenWidth * screenHeight;
	displayBuffer = new displayType[screenArea];
	Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CONSOLE_TEXTMODE_BUFFER, 
		NULL);

	for (int i = 0; i < screenArea; i++) {
		displayBuffer[i] = ' ';
	}

}


ConsolePlayingField::~ConsolePlayingField()
{
	delete displayBuffer; 
	CloseHandle(Console); 
}

void ConsolePlayingField::UpdateDisplayData(uint32_t* buffer) 
{
	for (int i = 0; i < screenArea; i++) {
		if (buffer[i]) {
			displayBuffer[i] = buffer[i]; 
		}
	}
}

void ConsolePlayingField::UpdateScore(uint32_t) 
{

}

void ConsolePlayingField::Draw() {
	WriteConsoleOutputCharacter(Console, 
		displayBuffer, 
		screenWidth * screenHeight, 
		{ 0,0 }, 
		&bytesWritten);

}
