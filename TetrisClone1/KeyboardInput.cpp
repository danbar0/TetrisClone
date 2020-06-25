#include "pch.h"
#include "KeyboardInput.h"
#include <Windows.h>

KeyboardInput::KeyboardInput()
{
	for (auto& input : inputs) {
		input.second = false; 
	}

	commandMap[Command::UP] = VK_UP;
	commandMap[Command::DOWN] = VK_DOWN;
	commandMap[Command::LEFT] = VK_LEFT;
	commandMap[Command::RIGHT] = VK_RIGHT;
	commandMap[Command::SPACE] = VK_SPACE;
}


KeyboardInput::~KeyboardInput()
{
}

IPlayerInput::inputs KeyboardInput::GetPlayerInputs() {
	int i = 0; 

	for (auto command : commandMap) {
		inputs[command.first] = GetAsyncKeyState(commandMap[command.first]);
	}

	//for (auto& input : inputs) {
	//	input.second = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x27\x25\x28Z\x20"[i]))) != 0;
	//	i++;
	//}

	return inputs; 
}
