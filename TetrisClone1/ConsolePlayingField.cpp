#include "pch.h"
#include "ConsolePlayingField.h"
#include <stdio.h>
#include <string>

/*-----------------------------------------------------------------------------------------------*/
ConsolePlayingField::ConsolePlayingField(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	bytesWritten = 0;
	screenArea = (screenWidth * screenHeight);
	displayBuffer = new displayType[screenArea];
	score = score.c_str();

	attributeMap.emplace(0, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	attributeMap.emplace('A', BACKGROUND_RED | BACKGROUND_INTENSITY);
	attributeMap.emplace('B', BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	attributeMap.emplace('C', BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	attributeMap.emplace('D', BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	attributeMap.emplace('E', BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	attributeMap.emplace('F', BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
	attributeMap.emplace('G', BACKGROUND_RED);

	Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		CONSOLE_TEXTMODE_BUFFER, 
		NULL);

	CONSOLE_CURSOR_INFO     cursorInfo;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 100; 
	SetConsoleCursorInfo(Console, &cursorInfo);

	SetConsoleTitle(TEXT("Console Tetris"));

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
	score = s2ws(std::to_string(buffer.clearedLines));
	scoreLength = strlen(std::to_string(buffer.clearedLines).c_str());
}

/*-----------------------------------------------------------------------------------------------*/
std::wstring ConsolePlayingField::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

/*-----------------------------------------------------------------------------------------------*/
void ConsolePlayingField::Draw() {
	DWORD written;
	SMALL_RECT rect = {
	0,
	0,
	 static_cast<SHORT>(screenWidth),
	 static_cast<SHORT>(screenHeight)
	};
	COORD scoreCoord{
	0,
	screenHeight + 1
	};
	wchar_t scoreBuffer[25];

	swprintf_s(scoreBuffer, sizeof(scoreBuffer)/sizeof(scoreBuffer[0]), L"LINES CLEARED: %s", score.c_str());

	WriteConsoleOutputCharacter (
		Console,
		scoreBuffer,
		wcslen(scoreBuffer),
		scoreCoord,
		&written
	);
	  
	int returnVal = WriteConsoleOutput(
		Console, 
		displayBuffer, 
		{ static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) },
		{0, 0},
		&rect
	);
}
