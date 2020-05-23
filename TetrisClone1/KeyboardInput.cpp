#include "pch.h"
#include "KeyboardInput.h"
#include <Windows.h>

KeyboardInput::KeyboardInput()
{
	for (auto& input : inputs) {
		input.second = false; 
	}
}


KeyboardInput::~KeyboardInput()
{
}

inputs KeyboardInput::GetPlayerInputs() {
	int i = 0; 

	for (auto& input : inputs) {
		input.second = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[i]))) != 0;
		i++;
	}

	return inputs; 
}
