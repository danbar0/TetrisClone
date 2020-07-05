#include "pch.h"
#include "ConsolePlayingField.h"
#include <stdio.h>

/*-----------------------------------------------------------------------------------------------*/
ConsolePlayingField::ConsolePlayingField(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	bytesWritten = 0;
	screenArea = (screenWidth * screenHeight);
	displayBuffer = new displayType[screenArea];
	score = 0; 

	attributeMap.emplace(0, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	attributeMap.emplace('A', BACKGROUND_RED | BACKGROUND_INTENSITY);
	attributeMap.emplace('B', BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	attributeMap.emplace('C', BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	attributeMap.emplace('D', BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	attributeMap.emplace('E', BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	attributeMap.emplace('F', BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
	attributeMap.emplace('G', 0);

	for (int i = 0; i < screenArea; i++) {
		displayBuffer[i].Char.UnicodeChar = ' ';
		displayBuffer[i].Attributes = BACKGROUND_GREEN | BACKGROUND_INTENSITY | COMMON_LVB_GRID_RVERTICAL;

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

/*-----------------------------------------------------------------------------------------------*/
ConsolePlayingField::~ConsolePlayingField()
{
	delete displayBuffer; 
	CloseHandle(Console); 
}

/*-----------------------------------------------------------------------------------------------*/
void ConsolePlayingField::UpdateDisplayBuffer(Buffer& buffer)
{
	for (int i = 0; i < buffer.field.size(); i++) {
		displayBuffer[i].Char.UnicodeChar = 0; 
		displayBuffer[i].Attributes = attributeMap[buffer.field[i]];
	}
}

/*-----------------------------------------------------------------------------------------------*/
void ConsolePlayingField::UpdateScore(uint32_t newScore) 
{
	score = newScore;
}

/*-----------------------------------------------------------------------------------------------*/
void ConsolePlayingField::Draw() {
	//swprintf_s(&screen[2 * screenWidth + ieldWidth + 6], 16, L"SCORE: %8d", score);
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
