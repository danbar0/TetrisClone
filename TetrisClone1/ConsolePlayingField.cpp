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
	score = 0; 

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

void ConsolePlayingField::UpdateDisplayBuffer(std::vector<uint8_t> buffer)
{
	for (int i = 0; i < screenArea; i++) {
		if (buffer[i]) {
			displayBuffer[i] = buffer[i]; 
		}
	}
}

void ConsolePlayingField::UpdateScore(uint32_t newScore) 
{
	score = newScore;
}

void ConsolePlayingField::Draw() {
	//swprintf_s(&screen[2 * screenWidth + ieldWidth + 6], 16, L"SCORE: %8d", score);

	int returnVal = WriteConsoleOutputCharacter(Console, 
		displayBuffer, 
		screenWidth * screenHeight, 
		{ 1,1 }, 
		&bytesWritten);
}
