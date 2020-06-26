#include "pch.h"
#include "ConsolePlayingField.h"
#include <stdio.h>


ConsolePlayingField::ConsolePlayingField(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	bytesWritten = 0;
	screenArea = (screenWidth * screenHeight);
	displayBuffer = new displayType[screenArea];
	score = 0; 

	for (int i = 0; i < screenArea; i++) {
		displayBuffer[i].Char.UnicodeChar = ' ';
		displayBuffer[i].Attributes = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	}

	Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CONSOLE_TEXTMODE_BUFFER, 
		NULL);

	CONSOLE_CURSOR_INFO     cursorInfo;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 100; 
	SetConsoleCursorInfo(Console, &cursorInfo);

	SetConsoleActiveScreenBuffer(Console);
}


ConsolePlayingField::~ConsolePlayingField()
{
	delete displayBuffer; 
	CloseHandle(Console); 
}

void ConsolePlayingField::UpdateDisplayBuffer(buffer buffer)
{
	for (int i = 0; i < buffer.size(); i++) {
		displayBuffer[i].Char.UnicodeChar = buffer[i]; 
	}
}

void ConsolePlayingField::UpdateScore(uint32_t newScore) 
{
	score = newScore;
}

void ConsolePlayingField::Draw() {
	////swprintf_s(&screen[2 * screenWidth + ieldWidth + 6], 16, L"SCORE: %8d", score);
	SMALL_RECT rect =  {
		0,
		0,
		 static_cast<SHORT>(screenWidth),
		 static_cast<SHORT>(screenHeight)
	};
	
	int returnVal = WriteConsoleOutput(
		Console, 
		displayBuffer, 
		{ static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) },
		{0, 0},
		&rect
	);
}
