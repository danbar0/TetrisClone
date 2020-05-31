#include "pch.h"
#include "ConsolePlayingField.h"
#include <stdio.h>


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
	SetConsoleActiveScreenBuffer(Console);

	for (int i = 0; i < screenArea; i++) {
		displayBuffer[i] = ' ';
	}

}


ConsolePlayingField::~ConsolePlayingField()
{
	delete displayBuffer; 
	CloseHandle(Console); 
}

void ConsolePlayingField::UpdateDisplayData(uint32_t* buffer, size_t size) 
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
	int returnVal = WriteConsoleOutputCharacter(Console, 
		displayBuffer, 
		screenWidth * screenHeight, 
		{ 1,1 }, 
		&bytesWritten);
}
