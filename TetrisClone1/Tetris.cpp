#include "pch.h"
#include "Tetris.h"
#include "Gameplay.h"

Tetris::Tetris(IPlayerInput& input, IPlayingField& display, timeTickDelayFunc delay) :
	playerInput(input),
	display(display),
	delay(delay)
{
	timeTicks = 1; 

	states.emplace(State::Game, new Gameplay(display.GetWidth(), display.GetHeight()));

	currentState = states[State::Game]; 

	for (int y = 0; y < display.GetHeight(); y++) { 
		for (int x = 0; x < display.GetWidth(); x++) { 
			displayBuffer.push_back(' '); 
		} 
	} 

} 

Tetris::~Tetris() 
{
	
}

void Tetris::Run()
{
	while (true)
	{
		updateTime();
		updateInputs();
		updateState();
		updateDisplay();
	}
}

void Tetris::updateTime() {
	delay(); 

	timeTicks++;
	if (timeTicks == 60001) timeTicks = 1; 
}

void Tetris::updateInputs()
{
	keys = playerInput.GetPlayerInputs();
}

void Tetris::updateState()
{
	currentState->Update(displayBuffer, keys, timeTicks); 
}

void Tetris::updateDisplay()  
{
	display.UpdateDisplayBuffer(displayBuffer);
	display.Draw();
}