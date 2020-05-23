#pragma once
#include "IPlayingField.h"
#include <Windows.h>
#include <thread>

class ConsolePlayingField : public IPlayingField
{
public:
	ConsolePlayingField(int screenWidth, int screenHeight);
	~ConsolePlayingField();

private:
	int screenWidth;
	int screenHeight;

	std::unique_ptr<wchar_t[]> displayBuffer;
	HANDLE console;
	DWORD bytesWritten; 
};

